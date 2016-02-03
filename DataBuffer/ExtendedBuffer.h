#ifndef EXTENDEDBUFFER_H
#define EXTENDEDBUFFER_H
//#include <iostream>
#include <cstdlib>
#include <iostream>
//#include <new>
#include "DataBuffer.h"

using namespace std;

/**
 * \brief Extends the DataBuffer class to handle data merging
 **/
template <typename T>
class ExtendedBuffer : public DataBuffer<T>
{
   protected:
      size_t m_elementCount = 0;                   //!< Number of elements set in the array
   public:
      void   deallocate();
      size_t getElementCount();
      size_t getElements( T * dest, size_t count, size_t startIndex = 0);
      size_t setElements( T * array, size_t count, size_t startIndex = 0, bool resizeFlag = false);
      size_t assignElements( T element, size_t count, size_t startIndex = 0, bool resizeFlag = false );
};

/**
 * \brief Returns the number of elements assigned to the array
 * \return number of elements assigned to the array
 **/
template<typename T>
size_t ExtendedBuffer<T>::getElementCount()
{
   return m_elementCount;
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
   m_elementCount = 0;

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
   if( count > m_elementCount - startIndex ) {
      count = m_elementCount - startIndex;
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
 * available space will be filled.
 **/
template<typename T>
size_t ExtendedBuffer<T>::setElements( T * array, size_t count, size_t startIndex, bool resizeFlag ) 
{
   //Make sure the input array is valid
   if(( array = NULL )||(count == 0)) {
      cerr<<"DataBuffer::setElements received a NULL input array"<<endl;
      return 0;
   }

   //Check to make sure we are bounded correctly. If not, resize if flag is set
   if( count +startIndex > DataBuffer<T>::m_allocatedElements ) {
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
   //Check to make sure we are bounded correctly. If not, resize if flag is set
   if( count +startIndex > DataBuffer<T>::m_allocatedElements ) {
      if( resizeFlag) {
         DataBuffer<T>::allocate(count+startIndex, true);
      }
      else {
         count = DataBuffer<T>::m_allocatedElements - startIndex;
      }
   }

   //Loop to copy the given number of elements
   for( int i = 0; i < count; i++ ) {
      DataBuffer<T>::m_buffer[startIndex+i] = element;
   }

}
//Test function
bool testExtendedBuffer();


#endif
