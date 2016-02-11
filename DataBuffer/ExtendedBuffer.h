#ifndef EXTENDEDBUFFER_H
#define EXTENDEDBUFFER_H
//#include <iostream>
#include <cstdlib>
#include <iostream>
#include <climits>
#include <cstring>
#include "TypeBuffer.h"
#include "DataBuffer.h"

using namespace std;

/**
 * \brief Extends the DataBuffer class to manage arrays of data.
 *
 * This class is templated so data can be accessed in terms of elements
 * as opposed to being restricted to a single type
 **/
template <typename T>
class ExtendedBuffer : public DataBuffer
{
   protected:
      uint8_t m_elementSize = sizeof(T);        //!< Base size of the template elment
      size_t  m_maxIndex = 0;                   //!< Number of elements set in the array
      size_t  m_elementCount = 0;               //!< Tracks the number of elements in the buffer
   public:
      ExtendedBuffer(size_t elements=0);

      bool   allocate( size_t elements, bool resizeFlag = false);
      void   deallocate();
      size_t getElementCount();
      size_t getMaxIndex();
      bool   setMaxIndex( size_t value );
      size_t setElements( T * array, size_t count, size_t startIndex = 0, bool resizeFlag = false);
      size_t getElements( T * dest, size_t count, size_t startIndex = 0);
      size_t assignElements( T element, size_t count, size_t startIndex = UINT_MAX, bool resizeFlag = false );

      TypeBuffer<T> getTypeBufferAndFree();


      /** \brief returns the value at the index **/
      uint8_t    operator [](size_t index) const   {return m_buffer[index*m_elementSize];}; 
      /** \brief assigned the index to the value **/
      uint8_t    & operator [](size_t index) {return m_buffer[index*m_elementSize];}; 

};

/**
 * \brief Sets the number of elements to the specified value
 *
 * \param [in] elements number of elements to allocate on creation (default = 0 );
 **/
template<typename T>
ExtendedBuffer<T>::ExtendedBuffer( size_t elements) 
{
   DataBuffer::allocate(elements * m_elementSize);
}

/**
 * \brief Sets the number of elements to the specified value
 * \return number of elements assigned to the array
 **/
template<typename T>
bool ExtendedBuffer<T>::setMaxIndex( size_t value) 
{
   //We cannot force an element count to be greater than
   //the number of allocated elements
   if( value > m_elementCount) {
      return false;
   }

   //We're in range. Set teh value
   m_maxIndex = value;
   return true;
}

/**
 * \brief Returns the number of elements allocated to the array
 * \return number of elements allocated in the array
 **/
template<typename T>
size_t ExtendedBuffer<T>::getElementCount()
{
   return m_elementCount;
}

/**
 * \brief Returns the number of elements assigned to the array
 * \return number of elements assigned to the array
 **/
template<typename T>
size_t ExtendedBuffer<T>::getMaxIndex()
{
   return m_maxIndex;
}

/**
 * \brief allocates the buffer and sets the elementCount
 *
 * \param [in] elements number of elements to allocate
 * \param [in] resizeFlag boolean to indicate if we resize as needed
 * \return true on success, false on failure
 *
 * This functions  wraps the DataBuffer::allocate call to provide a method
 * to set the element count. This data will automatically be allocated if
 * the underlying buffer is set to NULL
 **/
template<typename T>
bool ExtendedBuffer<T>::allocate( size_t elements, bool resizeFlag )
{
   bool rc = DataBuffer::allocate( elements * m_elementSize, resizeFlag );
   if(( rc )&&( elements > m_elementCount )) {
      m_elementCount = elements;
   }

   return rc;
}

/**
 * \brief deallocates the buffer and clears the elementCount
 *
 * \return true on success, false on failure
 *
 * This functions  wraps the DataBuffer::deallocate call to provide a method
 * to clear the element count.
 **/
template<typename T>
void ExtendedBuffer<T>::deallocate()
{
   DataBuffer::deallocate();
   m_maxIndex = 0;

   return;
}

/**
 * \brief This function copies an array of elements to the specified offset
 *
 * \param [in] array pointer to the array of elements to copy
 * \param [in] count number of elements to copy
 * \param [in] startIndex index to start to copy to
 * \param [in] resizeFlag flag to indicate if array should be resized if needed.
 * \return number of elements copied.
 *
 * If the resize flag is set, this function well expand the array to handle all
 * of the input data. Otherwise, if the internal buffer is undersized, all 
 * available space will be filled. If startIndex == UINT_MAX, the array is appended to 
 * the current m_maxIndex index. 
 **/
template<typename T>
size_t ExtendedBuffer<T>::setElements( T * array, size_t count, size_t startIndex, bool resizeFlag ) 
{
   size_t bytes = count * m_elementSize;
   bytes = DataBuffer::setData( array, bytes, startIndex * m_elementSize, resizeFlag );

   return bytes/m_elementSize;
}
/*
 * \brief This function sets the number of elements at the specified offset to the given value.
 *
 * \param [in] element Element to copy to the array
 * \param [in] count number of elements to copy
 * \param [in] startIndex index to start to copy to
 * \param [in] resizeFlag flag to indicate if array should be resized if needed.
 * \return number of elements copied.
 *
 * If the resize flag is set, this function well expand the array to handle all
 * of the input data. Otherwise, if the internal buffer is undersized, all 
 * available space will be filled.
 *
template<typename T>
size_t ExtendedBuffer<T>::assignElements( T element, size_t count, size_t startIndex, bool resizeFlag )
{
   //If we're set to UINT_MAX, the start with the highest element
   if( startIndex == UINT_MAX ) {
      startIndex = m_maxIndex;
   size_t result = DataBuffer::setData( (void *)array, bytes, startIndex*m_elementSize, resizeFlag );

   return result;
}
*/

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
size_t ExtendedBuffer<T>::getElements( T * dest, size_t count, size_t startIndex ) 
{
   if( count > m_maxIndex - startIndex ) {
      count = m_maxIndex - startIndex;
   }

   DataBuffer::getData( dest, count * m_elementSize, startIndex*m_elementSize );
   
   return count; 
}



/**
 * \brief This function sets the number of elements at the specified offset to the given value.
 *
 * \param [in] element Element to copy to the array
 * \param [in] count number of elements to copy
 * \param [in] startIndex index to start to copy to
 * \param [in] resizeFlag flag to indicate if array should be resized if needed.
 * \return number of elements copied.
 *
 * If the resize flag is set, this function well expand the array to handle all
 * of the input data. Otherwise, if the internal buffer is undersized, all 
 * available space will be filled.
 **/
template<typename T>
size_t ExtendedBuffer<T>::assignElements( T element, size_t count, size_t startIndex, bool resizeFlag )
{
   //If we're set to UINT_MAX, the start with the highest element
   if( startIndex == UINT_MAX ) {
      startIndex = m_maxIndex;
   }

   //Check to make sure we are bounded correctly. If not, resize if flag is set
   if( count +startIndex > m_elementCount ) {
      if(( resizeFlag )||(m_buffer == NULL )) {
         allocate(count+startIndex, resizeFlag );
      }
      else {
         count = m_elementCount - startIndex;
      }
   }
    

   T * mem = (T *)m_buffer;

   //Loop to copy the given number of elements
   for( int i = 0; i < count; i++ ) {
      mem[startIndex+i] = element;
   }

   if( startIndex+count > m_maxIndex ) {
      m_maxIndex = startIndex+count;
   }

   return count;
}

/**
 * \brief Gets a pointer to the buffer and clears class values (no deallocatoin)
 * \return TypeBuffer with the data pointer and number fo elements
 **/
template <typename T>
TypeBuffer<T> getTypeBufferAndFree()
{
   //Create the typebuffer with the output info
   TypeBuffer<T> tbuffer;
   tbuffer.m_buffer = static_cast<T *>(DataBuffer::m_buffer);
   tbuffer.m_elements = m_maxIndex;

   //Clear Extended Buffer variables
   m_maxIndex = 0;
   m_elementCount = 0;

   //Clear DataBuffer variables
   DataBuffer::m_buffer = NULL;
   DataBuffer::m_bufferSize = 0;
 
   return tbuffer;  
}

//Test function
bool testExtendedBuffer();


#endif
