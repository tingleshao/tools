#include <iostream>
#include <cstdlib>
#include <cstring>

#include "DataBuffer.h"

using namespace std;

namespace atl {
   /**
    * \brief Allocates the buffer to the given number of elements. 
    *
    * \param [in] bytes number of elements of the base type to allocate
    * \param [in] resizeFlag flag to indicate if data should be deleted and reallocated
    * \return true on success, false on failure
    **/
   bool RawDataBuffer::allocate( size_t bytes, bool resizeFlag)
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
   void RawDataBuffer::deallocate( void )
   {
      //Create a shared ptr to replace with current
      std::shared_ptr<uint8_t> tmpBuffer;                 //!< Actual data buffer

      swap( m_buffer, tmpBuffer );
      m_bufferSize = 0;
   }
   
   
   
   /**
    * \brief Constructor
    * 
    * \param[in] bytes number of bytes to allocate on start
    **/
   DataBuffer::DataBuffer( size_t bytes) 
   {
      if( bytes > 0 ) {
         allocate(bytes);
      }
   }
   
   /**
    * \brief Destructor
    *
    * Makes sure all allocated data gets removed
    **/
   DataBuffer::~DataBuffer() 
   {
      if( m_buffer != NULL ) {
         deallocate();
      }
   }
   
   /**
    * \brief Allocates data to the given size and clear memory based on useDefaultValueFlag
    **/
   bool DataBuffer::allocate( size_t bytes, bool resizeFlag ) 
   {
      size_t origOffset = m_bufferSize;
      bool rc = RawDataBuffer::allocate( bytes, resizeFlag );
      if(( rc )&&(m_useDefaultValueFlag)) {
         for( size_t i = origOffset; i < m_bufferSize; i++ )  {
            m_buffer.get()[i] = m_defaultValue;
         }
      }
   
      return rc;
   }
   
   /**
    * \brief Sets the default value to assign to data
    * \param [in] value default value to set all allocated data to
    *
    * This function sets the default value to set all allocated data to
    * and sets the useDefaultValueFlag to true. All future allocated 
    * bytes will be set to this value on allocatoin. The useDefaultValue 
    * function can override the useDefaultValue flag
    **/
   void DataBuffer::setDefaultValue( uint8_t value ) 
   {
      m_defaultValue = value;
      m_useDefaultValueFlag = true;
   }
   
   /**
    * \brief Returns a copy of data from the buffer
    *
    * \param [in] offset offset from the start to begin copy
    * \return RawDataBuffer with a pointer to the memory buffer and the size of the data
    *
    * This function instantiates a RawDataBuffer and populates it with data from the
    * class. It is assumed the calling process will delete the data when it is complete.
    **/
   RawDataBuffer DataBuffer::getData()
   {
      RawDataBuffer rawData;
   
      //Make sure we have data
      if( m_bufferSize == 0 ) {
         return rawData;
      }

      rawData.m_buffer = m_buffer;
      rawData.m_bufferSize = m_bufferSize;
   
      //Allocate the datastruct
//      rawData.allocate(m_bufferSize);
//      getData( &rawData.m_buffer, &rawData.m_bufferSize);
   
      return rawData;
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
   size_t DataBuffer::setData( void * array, size_t count, size_t offset, bool resizeFlag ) 
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
         for( int i = 0; i < count; i++) {
            m_buffer.get()[offset+i] = static_cast<uint8_t *>(array)[i];
         }
      }
   
      return count;
   }
   
   /**
    * \brief Returns a copy of the elements at the given destination
    *
    * \param [in] buffer shared pointer to the buffer
    * \param [in] bytes number of bytes copied
    * \param [in] startIndex index into array to start with (default=0)
    * \return number of elements successfully copied
    *
    * This function copies the number of specified elements to the destination
    * address starting at the index specified by the startIndex. If startIndex 
    * is greater that the number of specified elements (bufferSize) this only
    * the elements that are defined are copied. 
    *
    * No error checking is performed to ensure that the destination pointer has
    * been properly allocated.
    **/
   bool DataBuffer::getData( std::shared_ptr<uint8_t> &buffer, size_t &bytes) 
   {
      buffer = m_buffer;
      bytes  = m_bufferSize;
      return true; 
   }
   
   /**
    * \brief Returns the number of elements allocated in the array.
    * \return number of allocated elements
    **/
   size_t DataBuffer::getSize(void)
   {
      return m_bufferSize;
   }
   
   /** 
    * \brief This function is used to enable or disable setting allocated data to the default value
    *
    **/
   void DataBuffer::useDefaultValue(bool flag )
   {
      m_useDefaultValueFlag = flag;
   }
 }
   
   
   
   
   /**
    * \brief Unit test for DataBuffer functionality
    **/
   bool testDataBuffer()
   {
      bool rc = true;
   
      int defaultValue = 10;
      size_t elements = 100;
   
      //Create some buffers
      atl::DataBuffer dataBuffer;
      
      //Specify what the defautl value is
      dataBuffer.setDefaultValue(defaultValue );
   
      //Allocate buffers
      dataBuffer.allocate(elements, true);
   
      if( dataBuffer.m_buffer.get()[0] != defaultValue ) {
         std::cerr << "Default value not working: "<<(int)dataBuffer[0]<<"!="<< defaultValue <<endl;
         return false;
      }
   
   
      size_t count = dataBuffer.getSize();
      if( count != elements ) {
         std::cerr << "Number of allocated elements does not match:\n\t" 
                   <<count<<"!="<<elements << endl;
         rc = false;
      }
   
      for( int i = -2; i < 2; i++ ) {
         dataBuffer[i+2] = (uint8_t)i;
      }
   
      for( int i = -2; i < 2; i++ ) {
         if( dataBuffer[i+2] != (uint8_t)i ) {
            std::cerr << "DataBuffer mismatch"<<std::endl;
            rc = false;
         }
      }
   
      //Create a new one to add an array
      atl::DataBuffer dataBuffer2;
      std::shared_ptr<uint8_t> buffer2;
      uint8_t buffer[elements];
      for( int i = 0; i <elements; i++ ) 
      {
         buffer[i] = i;
      }
   
      //Try to add elements with default values
      size_t result = dataBuffer2.setData(buffer, elements, 0, true);
      if( result != elements ) {
         std::cerr<<"Wrote "<<result<<" elements when expecting "<<elements<<std::endl;
         return false;
      }
   
      //Read elements from buffer
      dataBuffer2.getData( buffer2, result);
   
      if( result != elements ) {
         std::cerr<<"getData did not return correct number of elements. "
                  << result << "!=" << elements << std::endl;
      }
   
      rc = true;
      for( int i = 0; i < elements; i++ ) {
         if( buffer[i] != buffer2.get()[i] ) {
            std::cerr<<"Buffer "<<i<<" does not match output:("<<buffer[i]<<"!="<<buffer2.get()[i]<<")"<<std::endl;
            rc = false;
         }
      }
   
      if( !rc ) {
         std::cerr<<"Failed to set/get elements"<<std::endl;
      }
   
      atl::RawDataBuffer rdb = dataBuffer2.getData();
   
      rdb.deallocate();
   
      //Deallocate buffers
      dataBuffer.deallocate();
   
      return rc;
   }
