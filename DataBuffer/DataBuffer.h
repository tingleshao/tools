#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <cstdlib>
#include <iostream>
#include <climits>
#include <cstring>

/**
 * \brief Low level data structure to associate pointer with a data size
 *
 * This class is a very simple interface to represent a continuous array of data.
 * The size value is the number of elements in the buffer and the buffer is the data
 * itself. There is no inherent method for data management allocated data is must be
 * freed with an external call to the deallocate function
 **/
template<typename T>
struct RawDataStruct
{
   size_t size   =0;                     //!< Number of elements in the buffer
   T*     buffer = NULL;                 //!< Actual data buffer

   void allocate( size_t size);
   void deallocate();
};

/**
 * !\brief Allocated the buffer
 *
 * This function reallocates the buffer to the given size
 **/
template<typename T>
void RawDataStruct<T>::allocate( size_t newSize ) 
{
   if( buffer != NULL ) {
      deallocate();
   }

   buffer = new T(newSize);
}

/**
 * !\brief Released the buffer
 **/
template<typename T>
void RawDataStruct<T>::deallocate() 
{
   delete[] buffer;
   buffer = NULL;
}



/**
 * \brief Base class to handle data management for abstract data types
 **/
template <typename T>
class DataBuffer
{
   protected:
      size_t   m_bufferSize   = 0;            //!< Size of the allocated buffer in bytes
      size_t   m_elementCount = 0;            //!< Number of elements in the buffer based on T
      T        m_defaultValue = false;        //!< Default value
      bool     m_useDefaultValueFlag = false; //!< Flag to indicate if we should use default value
      T *      m_buffer = NULL;               //!< Pointer to buffer data

   public:

      DataBuffer(size_t elements=0);
      ~DataBuffer();
      
      /** \brief returns the value at the index **/
      T    operator [](size_t index) const   {return m_buffer[index];}; 
      /** \brief assigned the index to the value **/
      T    & operator [](size_t index) {return m_buffer[index];}; 

      void    useDefaultValue( bool flag);
      void    setDefaultValue(T value );
      size_t  setElements( T * array, size_t count, size_t startIndex = UINT_MAX, bool resizeFlag = false );
      size_t  getElements( T * dest, size_t count, size_t startIndex=0 );
      size_t  getElementCount();
      RawDataStruct<T> getData();

      virtual bool allocate( size_t elements, bool resizeFlag = false );
      virtual void deallocate();
};

using namespace std;

/**
 * \brief Constructor
 * 
 * \param[in] elements number of elements (default = 0)
 **/
template<typename T>
DataBuffer<T>::DataBuffer( size_t elements ) 
{
   if( elements > 0 ) {
      allocate(elements);
   }
}

/**
 * \brief Destructor
 *
 * Makes sure all allocated data gets removed
 **/
template<typename T>
DataBuffer<T>::~DataBuffer() 
{
   if( m_buffer != NULL ) {
      deallocate();
   }
}

/**
 * \brief Sets the default value to assign to data
 * \param [in] value default value to set all allocated data to
 *
 * This function sets the default value to set all allocated data to
 * and sets the useDefaultValueFlag to true. All future allocated 
 * elements will be set to this value. The useDefaultValue function 
 * can override the useDefaultValue flag
 **/
template<typename T>
void DataBuffer<T>::setDefaultValue( T value ) 
{
   m_defaultValue = value;
   m_useDefaultValueFlag = true;
}

/**
 * \brief Returns a copy of data from the buffer
 *
 * \return RawDataStruct with a pointer to the memory buffer and the size of the data
 *
 * This function instantiates a RawDataStruct and populates it with data from the
 * class. It is assumed the calling process will delete the data when it is complete.
 **/
template <typename T>
RawDataStruct<T> DataBuffer<T>::getData()
{
   RawDataStruct<T> rawData;

   //Make sure we have data
   if( m_elementCount == 0 ) {
      return rawData;
   }

   //Allocate the datastruct
   rawData.allocate(m_elementCount);
   getElements( rawData.buffer, rawData.size);

   return rawData;
}

/**
 * \brief This function copies an array of elements to the specified offset
 *
 * \param [in] array pointer to the array of elements to copy
 * \param [in] count number of elements to copy
 * \param [in] startIndex index to start to copy to. If set to MAX then adds more elements
 * \param [in] resizeFlag flag to indicate if array should be resized if needed.
 * \return number of elements copied.
 *
 * If the resize flag is set, this function well expand the array to handle all
 * of the input data. Otherwise, if the internal buffer is undersized, all 
 * available space will be filled. If startIndex == UINT_MAX, the array is appended to 
 * the current m_maxIndex index. 
 **/
template<typename T>
size_t DataBuffer<T>::setElements( T * array, size_t count, size_t startIndex, bool resizeFlag ) 
{
   //Make sure the input array is valid
   if(( array == NULL )||(count == 0)) {
      cerr<<"DataBuffer::setElements received a NULL input array"<<endl;
      return 0;
   }

   //If we're set to UINT_MAX, the start with the highest element
   if( startIndex == UINT_MAX ) {
      startIndex = m_elementCount;
   }

   //Check to make sure we are bounded correctly. If not, resize if flag is set
   if( count + startIndex > m_elementCount ) {
      if( resizeFlag) {
         allocate(count+startIndex, true);
      }
      else {
         count = m_elementCount - startIndex;
      }
   }

   size_t bytes = count * sizeof(T);
   if( bytes == 0 ) {
      return bytes;
   }
   
   //Perform copy
   std::memcpy(&m_buffer[startIndex], array, bytes);

   return count;
}

/**
 * \brief Returns a copy of the elements at the given destination
 *
 * \param [in] dest destination address to write data to
 * \param [in] count number of elements to copy 
 * \param [in] startIndex index into array to start with (default=0)
 * \return number of elements successfully copied
 *
 * This function copies the number of specified elements to the destination
 * address starting at the index specified by the startIndex. If startIndex 
 * is greater that the number of specified elements (elementCount) this only
 * the elements that are defined are copied. 
 *
 * No error checking is performed to ensure that the destination pointer has
 * been properly allocated.
 **/
template<typename T>
size_t DataBuffer<T>::getElements( T * dest, size_t count, size_t startIndex ) 
{
   if( count > m_elementCount - startIndex ) {
      count = m_elementCount - startIndex;
   }

   size_t bytes = count * sizeof(T);
   memcpy( dest, m_buffer, bytes );
   
   return count; 
}

/**
 * \brief Returns the number of elements allocated in the array.
 * \return number of allocated elements
 **/
template<typename T>
size_t DataBuffer<T>::getElementCount(void)
{
   return m_elementCount;
}

/** 
 * \brief This function is used to enable or disable setting allocated data to the default value
 *
 **/
template<typename T>
void DataBuffer<T>::useDefaultValue(bool flag )
{
   m_useDefaultValueFlag = flag;
}

/**
 * \brief Allocates the buffer to the given number of elements. 
 *
 * \param [in] elements number of elements of the base type to allocate
 * \param [in] resizeFlag flag to indicate if data should be deleted and reallocated
 * \return true on success, false on failure
 **/
template<typename T>
bool DataBuffer<T>::allocate( size_t elements, bool resizeFlag)
{
   //Check to make sure we are not already allocated
   if( m_buffer != NULL ) {
      if( !resizeFlag ) {
         return false;
      }
   }

   //Check if reallocating to zero. If so, deallocate
   if( elements == 0 ) {
      deallocate();
      return true;
   }

   size_t bytes = sizeof(T)*elements;

   void * mem = std::realloc(m_buffer, bytes);
   if( mem == NULL ) {
      throw std::bad_alloc();
   }

   m_buffer = static_cast<T*>(mem);

   //Assign default values to buffers
   if( m_useDefaultValueFlag ) {
      for(int i = m_elementCount; i < elements; i++)
      {
         m_buffer[i] = m_defaultValue;
      }
   }

   m_elementCount = elements;
   m_bufferSize = bytes;


   return true;
}

/**
 * \brief Deallocates and allocated data
 **/
template<typename T>
void DataBuffer<T>::deallocate( void )
{
   if( m_buffer == NULL ) {
      return;
   }

   std::free(m_buffer);
   m_buffer = NULL;
   m_elementCount = 0;
   m_bufferSize = 0;
}

// Function to test the databuffer operation
bool testDataBuffer(void);
#endif
