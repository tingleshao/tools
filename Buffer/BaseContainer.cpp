#include <iostream>
#include <sstream>
#include <string>

#include <fcntl.h>
#include <unistd.h>

#include "BaseContainer.h"

namespace atl
{
   /**
    * \brief allocates container data
    * \param [in] bytes      number of bytes data is expected to require
    * \param [in] blockSize  minimum size of a block in bytes (default = 1)
    * \return number of bytes avialable in the data buffer (0 indicates failure)
    *
    * This function allocates the buffer to the given size and assigns the appropriate ID.
    * To speed up file I/O and data management, the memory is stored as a continuous array 
    * with that is an integer multiple of the blockSize as specified by the block count. The 
    * first bytes in the memory contain the metadata pointer. 
    **/
    size_t BaseContainer::allocate( size_t bytes, size_t blockSize ) 
    {
       if( blockSize == 0 ) {
           std::cerr << "BaseContainer::allocate: BlockSize of 0 invalid"<<std::endl;
           return 0;
       }

       size_t totalBytes = bytes + sizeof( BaseContainerMetadata );

       m_blockSize  = blockSize;
       m_blockCount = (totalBytes+blockSize-1)/m_blockSize;

       //Allocate enough memory for the met
       if( !m_buffer.allocate( m_blockCount * m_blockSize )) {
          std::cerr << "BaseContainer: Failed to allocated "<<m_blockCount * m_blockSize<<" bytes."<<std::endl;
          return 0;
       }

       //Map metadat to the appropriate pointer
       m_metadata = (BaseContainerMetadata *)(&m_buffer[0]);
       m_metadata->m_offset = sizeof( BaseContainerMetadata);
       m_metadata->m_type   = TYPE_BASE;
       m_metadata->m_size = m_blockCount * m_blockSize;

       return m_metadata->m_size - m_metadata->m_offset;
    }
    
   /**
    * \brief Function to save the base container
    **/
   bool BaseContainer::save( std::string filename ) 
   {
      //Open the specified filename
      int fd = open( filename.c_str(), O_WRONLY |O_CREAT, 0777 );
      if( fd < 0 ) {
         std::cerr << "BaseContainer unable to open "<<filename.c_str()<<std::endl;
         return false;
      }

  
      //Write the header
      bool    rc = true;
      size_t  byteCount = 0;
      ssize_t result = 0;
      while((result >= 0 )&&( byteCount < m_blockCount * m_blockSize )) {
         result = write( fd, &m_buffer[0] + byteCount, m_blockCount * m_blockSize - byteCount ); 
         if( result < 0 ) {
            std::cerr << "Based Container failed while writing data " <<std::endl;
            rc = false;
         }
         else {
            byteCount += result;
         }
      }

      close(fd);
      return rc;
   }

   /**
    * \brief returns the size of the container
    **/
   size_t BaseContainer::getSize() 
   {
      if( m_metadata == NULL ) {
         return 0;
      }

      return m_metadata->m_size;
   }

   /**
    * \brief returns the size of the data buffer
    **/
   size_t BaseContainer::getDataSize()
   {
      return m_metadata->m_size - m_metadata->m_offset;
   }

   /**
    * \brief Returns a pointer to the head of the data
    **/
   void * BaseContainer::getDataPointer()
   {
      return &m_buffer[0]+m_metadata->m_offset;
   }

   /**
    * \brief Test function
    **/
   bool testBaseContainer() 
   {
      size_t bcSize = 100;
      BaseContainer container;

      //No space is allocated, so we should have no size
      size_t sz = container.getSize();
      if( sz != 0 ) {
         std::cout << "Invalid container size1: "<<sz << "!="<<0<<std::endl;
         return false;
      }
      container.allocate(bcSize);

      sz = container.getSize();
      if( sz != bcSize + sizeof(BaseContainerMetadata)) {
         std::cout << "Invalid container size2: "<<sz << "!="<<bcSize + sizeof( BaseContainerMetadata) <<std::endl;
         return false;
      }

      container.m_metadata->m_id = 1;

      std::string expected = "{\"id\":1,\"size\":140,\"offset\":40,\"type\":1}";
      std::string result   = container.m_metadata->getJsonString();

      if( result.compare(expected)) {
         std::cerr << "testBaseContainer failed:"<<result.c_str()<<"!="<<expected.c_str()<<std::endl;
         return false;
      }

      uint32_t * buffer = (uint32_t *)container.getDataPointer();
      size_t count = container.getDataSize() / sizeof(uint32_t);
      if( count != bcSize/sizeof(uint32_t)) {
         std::cerr << "Size mismatch:"<<count<<"!="<<bcSize/sizeof(uint32_t) << std::endl;
         return false;
      }
      for( uint32_t i = 0; i < count; i++ ) {
         buffer[i] = i;
      }

      for( uint32_t i = 0; i < count; i++ ) {
         if(buffer[i] != i ) {
            std::cerr << "buffer["<<i<<"] != "<<(char)i<<std::endl;
            return false;
         }
      }
      return true;
 
   }
};
