#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <cstdlib>
#include <iostream>

/**
 * \brief Base class to handle data management for abstract data types
 **/
template <typename T>
class DataBuffer
{
   protected:
      size_t   m_bufferSize     = 0;     //!< Size of the allocated buffer in bytes
      size_t   m_allocatedElements = 0;  //!< Number of elements in the buffer based on T
      T *      m_buffer = NULL;          //!< Pointer to buffer data
      T        defaultValue;             //!< Default value
      bool     useDefaultValueFlag = false;  //!< Flag to indicate if we should use default value

   public:
      ~DataBuffer();
      
      /** \brief returns the value at the index **/
      T    operator [](size_t index) const   {return m_buffer[index];}; 
      /** \brief assigned the index to the value **/
      T    & operator [](size_t index) {return m_buffer[index];}; 

      void    useDefaultValue( bool flag);
      void    setDefaultValue(T value );
      size_t  getAllocatedElements();
      virtual bool allocate( size_t elements, bool resizeFlag = false );
      virtual void deallocate();
};

using namespace std;
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
   defaultValue = value;
   useDefaultValueFlag = true;
}

/**
 * \brief Returns the number of elements allocated in the array.
 * \return number of allocated elements
 **/
template<typename T>
size_t DataBuffer<T>::getAllocatedElements(void)
{
   return m_allocatedElements;
}

/** 
 * \brief This function is used to enable or disable setting allocated data to the default value
 *
 **/
template<typename T>
void DataBuffer<T>::useDefaultValue(bool flag )
{
   useDefaultValueFlag = flag;
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
   if( useDefaultValueFlag ) {
      for(int i = m_allocatedElements; i < elements; i++)
      {
         m_buffer[i] = defaultValue;
      }
   }

   m_allocatedElements = elements;
   m_bufferSize = sizeof( bytes );


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
   m_allocatedElements = 0;
   m_bufferSize = 0;
}

// Function to test the databuffer operation
bool testDataBuffer(void);
#endif
