#ifndef EXTENDEDBUFFER_H
#define EXTENDEDBUFFER_H
//#include <iostream>
#include <cstdlib>
#include <iostream>
#include <climits>
#include "DataBuffer.h"

using namespace std;

/**
 * \brief Extends the DataBuffer class to handle data merging
 **/
template <typename T>
class ExtendedBuffer : public DataBuffer<T>
{
   protected:
      size_t m_maxIndex = 0;                   //!< Number of elements set in the array
   public:
      ExtendedBuffer(size_t elements=0);

      void   deallocate();
      size_t getMaxIndex();
      bool   setMaxIndex( size_t value );
      size_t getElements( T * dest, size_t count, size_t startIndex = 0);
      size_t setElements( T * array, size_t count, size_t startIndex = 0, bool resizeFlag = false);
      size_t assignElements( T element, size_t count, size_t startIndex = UINT_MAX, bool resizeFlag = false );
};

/**
 * \brief Sets the number of elements to the specified value
 *
 * \param [in] elements number of elements to allocate on creation (default = 0 );
 **/
template<typename T>
ExtendedBuffer<T>::ExtendedBuffer( size_t elements) 
{
   DataBuffer<T>::allocate(elements);
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
   if( value > DataBuffer<T>::getElementCount()) {
      return false;
   }

   //We're in range. Set teh value
   m_maxIndex = value;
   return true;
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
   DataBuffer<T>::deallocate();
   m_maxIndex = 0;

   return;
}

/**
 * \brief Returns a copy of the elements at the given destination
 *
 * \param [in] dest destination address to write data to
 * \param [in] count number of elements to copy
 * \param [in] startIndex index into array to start with
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

   size_t bytes = count & sizeof(T);
   memcpy( dest, DataBuffer<T>::m_buffer, bytes );
   
   return count; 
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
   //Make sure the input array is valid
   if(( array = NULL )||(count == 0)) {
      cerr<<"DataBuffer::setElements received a NULL input array"<<endl;
      return 0;
   }

   //If we're set to UINT_MAX, the start with the highest element
   if( startIndex == UINT_MAX ) {
      startIndex = m_maxIndex;
   }

   //Check to make sure we are bounded correctly. If not, resize if flag is set
   if( count +startIndex > DataBuffer<T>::m_elementCount ) {
      if( resizeFlag) {
         DataBuffer<T>::allocate(count+startIndex, true);
      }
      else {
         count = DataBuffer<T>::m_allocatedElements - startIndex;
      }
   }

   size_t bytes = count * sizeof(T);

   //Perform copy
   memcpy(&DataBuffer<T>::m_buffer[startIndex], array, count * sizeof(T));

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

   size_t elementCount = DataBuffer<T>::getElementCount();

   //Check to make sure we are bounded correctly. If not, resize if flag is set
   if( count +startIndex > elementCount ) {
      if( resizeFlag) {
         DataBuffer<T>::allocate(count+startIndex, true);
      }
      else {
         count = elementCount - startIndex;
      }
   }

   //Loop to copy the given number of elements
   for( int i = 0; i < count; i++ ) {
      DataBuffer<T>::m_buffer[startIndex+i] = element;
   }

   if( startIndex+count > m_maxIndex ) {
      m_maxIndex = startIndex+count;
   }

   return count;
}

//Test function
bool testExtendedBuffer();


#endif
