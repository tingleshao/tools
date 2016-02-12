#include <iostream>
#include <cstdlib>
#include <cstring>

#include "DataBuffer.h"

using namespace std;
/**
 * \brief Allocates the buffer to the given number of elements. 
 *
 * \param [in] bytes number of elements of the base type to allocate
 * \param [in] resizeFlag flag to indicate if data should be deleted and reallocated
 * \return true on success, false on failure
 **/
bool RawDataBuffer::allocate( size_t bytes, bool resizeFlag)
{
   //Check to make sure we are not already allocated
   if( m_buffer != NULL ) {
      if( !resizeFlag ) {
         return false;
      }
   }

   //Check if reallocating to zero. If so, deallocate
   if( bytes == 0 ) {
      deallocate();
      return true;
   }

   if( bytes > m_bufferSize ) {
      void * mem = std::realloc(m_buffer, bytes);
      if( mem == NULL ) {
         throw std::bad_alloc();
      } 
      else {
         m_buffer = static_cast<uint8_t *>(mem);
         m_bufferSize = bytes;
      }
   }

   return true;
}

/**
 * \brief Deallocates and allocated data
 **/
void RawDataBuffer::deallocate( void )
{
   if( m_buffer == NULL ) {
      return;
   }

   std::free(m_buffer);
   m_buffer = NULL;
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
      std::memset( m_buffer+origOffset, m_defaultValue, bytes-origOffset );
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
RawDataBuffer DataBuffer::getData( size_t offset)
{
   RawDataBuffer rawData;

   //Make sure we have data
   if( m_bufferSize == 0 ) {
      return rawData;
   }

   //Allocate the datastruct
   rawData.allocate(m_bufferSize);
   getData( rawData.m_buffer, rawData.m_bufferSize);

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
      if( resizeFlag) {
         allocate(count+offset, true);
      }
      else {
         count = m_bufferSize - offset;
      }
   }

   //Perform copy
   std::memcpy(&m_buffer[offset], array, count);

   return count;
}

/**
 * \brief Returns a copy of the elements at the given destination
 *
 * \param [in] dest destination address to write data to
 * \param [in] bytes number of elements to copy 
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
size_t DataBuffer::getData( void * dest, size_t bytes, size_t startIndex ) 
{
   if( bytes > m_bufferSize - startIndex ) {
      bytes = m_bufferSize - startIndex;
   }

   memcpy( dest, m_buffer, bytes );
   
   return bytes; 
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





// Function to test the databuffer operation
bool testDataBuffer(void);



/**
 * \brief Unit test for DataBuffer functionality
 **/
bool testDataBuffer()
{
   bool rc = true;

   int defaultValue = 10;
   int elements = 100;

   //Create some buffers
   DataBuffer dataBuffer;
   
   dataBuffer.setDefaultValue(defaultValue );

   //Allocate buffers
   dataBuffer.allocate(elements, true);

   if( dataBuffer[0] != defaultValue ) {
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
   DataBuffer dataBuffer2;
   uint8_t buffer[elements];
   uint8_t buffer2[elements];
   for( int i = 0; i <elements; i++ ) 
   {
      buffer[i] = i;
   }

   //Try to add elements with default values
   size_t result = dataBuffer2.setData(buffer, elements);
   if( result != 0 ) {
      std::cerr<<"Wrote "<<result<<" elements when expecting 0"<<std::endl;
      return false;
   }

   //Try to add elements with default values
   result = dataBuffer2.setData(buffer, elements, 0, true);
   if( result != elements ) {
      std::cerr<<"Wrote "<<result<<" elements when expecting "<<elements<<std::endl;
      return false;
   }

   //Read elements from buffer
   result = dataBuffer2.getData( buffer2, elements);

   if( result != elements ) {
      std::cerr<<"getData did not return corrent number of elements. "
               << result << "!=" << elements << std::endl;
   }

   rc = true;
   for( int i = 0; i < elements; i++ ) {
      if( buffer[i] != buffer2[i] ) {
         std::cerr<<"Buffer "<<i<<" does not match output:("<<buffer[i]<<"!="<<buffer2[i]<<")"<<std::endl;
         rc = false;
      }
   }

   if( !rc ) {
      std::cerr<<"Failed to set/get elements"<<std::endl;
   }

   RawDataBuffer rdb = dataBuffer2.getData();

   rdb.deallocate();

   //Deallocate buffers
   dataBuffer.deallocate();

   return rc;
}

