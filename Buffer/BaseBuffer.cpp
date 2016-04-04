/**a
 * \file BaseBuffer.cpp
 *
 * \copyright 2016 Aqueti, Incorporated
 * \license The MIT License (MIT)
 **/


#include <iostream>
#include <cstdlib>
#include <cstring>

#include "BaseBuffer.h"



using namespace std;

namespace atl {
   /**
    * \brief Allocates the buffer to the given number of elements. 
    *
    * \param [in] bytes number of elements of the base type to allocate
    * \param [in] resizeFlag flag to indicate if data should be deleted and reallocated
    * \return true on success, false on failure
    **/
   bool BaseBuffer::allocate( size_t bytes, bool resizeFlag)
   {
      //Make sure buffer is not already allocated
      size_t bufferSize = 0;

      //If we are not resizing and data exists, delete data
      if((resizeFlag == false )&&(m_bufferSize > 0 )) {
         return false;
      }

      if( bytes == 0 ) {
         deallocate();
         return true;
      }

      //Make a copy of the 
      std::shared_ptr<uint8_t> buffer; 
      if( m_bufferSize > 0 ) {
         buffer.swap( m_buffer ); 
         bufferSize = m_bufferSize;
      }

      m_buffer.reset( static_cast<uint8_t *>(std::malloc(bytes)), std::free );
      m_bufferSize = bytes;

      size_t copySize = bytes;
      if( bufferSize < bytes ) { 
         copySize = bufferSize;
      }
      memcpy( m_buffer.get(), buffer.get(), copySize );

      return true;
   }
   
   /**
    * \brief Deallocates and allocated data
    **/
   void BaseBuffer::deallocate()
   {
      //Create a shared ptr to replace with current
      std::shared_ptr<uint8_t> tmpBuffer;                 //!< Actual data buffer

      swap( m_buffer, tmpBuffer );
      m_bufferSize = 0;
   }

   /**
    * \brief Returns the allocated size of the buffer
    **/
    size_t BaseBuffer::getSize()
    {
       return m_bufferSize;
    }

   /**
    * \brief This function copies an array of elements to the specified offset
    *
    * \param [in] array pointer to the array of elements to copy
    * \param [in] count number of bytes to copy
    * \param [in] offset offset to start to copy from. If set to UINT_MAX then it appends to the end
    * \param [in] resizeFlag flag to indicate if array should be resized if needed.
    * \return number of elements copied.
    *
    * If the resize flag is set, this function will expand the array to handle all
    * of the input data. Otherwise, if the internal buffer is undersized, all 
    * available space will be filled. If offset == UINT_MAX, the array is appended to 
    * the current array. 
    **/
   size_t BaseBuffer::setData( void * array, size_t count, size_t offset, bool resizeFlag )
   {
      //Make sure the input array is valid
      if(( array == NULL )||(count == 0)) {
         cerr<<"DataBuffer::setElements received a NULL input array"<<endl;
         return 0;
      }

      //If we're set to UINT_MAX, the start with the highest element
      if( offset == UINT_MAX ) {
         offset = m_bufferSize;
      }

      //Check to make sure we are bounded correctly. If not, resize if flag is set
      if( count + offset > m_bufferSize ) {
         allocate(count+offset, true);
      }
      else {
         count = m_bufferSize - offset;
      }

      //Perform copy
      if( offset+count <= m_bufferSize ) {
         memcpy( &m_buffer.get()[offset], array, count );
      }

      return count;
   }


   /**
    * \brief Unit test for DataBuffer functionality
    **/
   bool testBaseBuffer()
   {
      BaseBuffer baseBuffer;
      if( baseBuffer.getSize()!= 0 ) {
         std::cerr << "Default baseBuffer size should be 0"<<std::endl;
         return false;
      }

      baseBuffer.allocate(100);
      if( baseBuffer.getSize() != 100 ) {
         std::cerr << "Default baseBuffer size should be 100"<<std::endl;
         return false;
      }

      baseBuffer[10] = 10;
      if( baseBuffer[10] != 10 ) {
         std::cerr << "Default baseBuffer element not set to 10"<<std::endl;
         return false;
      }
     
      baseBuffer[10] = 11;
      if( baseBuffer[10] != 11 ) {
         std::cerr << "Default baseBuffer element not set to 11"<<std::endl;
         return false;
      }

      baseBuffer.deallocate();
      if( baseBuffer.getSize()!= 0 ) {
         std::cerr << "Default baseBuffer size should be 0"<<std::endl;
         return false;
      }

      baseBuffer.allocate(100);
      if( baseBuffer.getSize() != 100 ) {
         std::cerr << "Default baseBuffer size should be 100"<<std::endl;
         return false;
      }

      return true;
   }
}
