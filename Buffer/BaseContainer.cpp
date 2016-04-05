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
    * \param [in] blockCount number of blocks in the container. 0 = unlimited (default = 0)
    * \param [in] blockSize  minimum size of a block in bytes (default = 1)
    * \return true on sucess, false on failure
    *
    * This function allocates the buffer to the given size and assigns the appropriate ID.
    * To speed up file I/O and data management, the memory is stored as a continuous array 
    * with that is an integer multiple of the blockSize as specified by the block count. The 
    * first bytes in the memory contain the metadata pointer. 
    **/
    bool BaseContainer::allocate( size_t blockCount, size_t blockSize ) 
    {
       m_blockCount    = blockCount;
       m_blockSize     = blockSize;

       //If blockCount is 0, calculate how many blocks we need to allocate for the metadata
       if( blockCount == 0 ) {
          m_blockCount = (sizeof(BaseContainerMetadata)-blockSize)/blockSize;
          m_blockSize = blockSize;
       }
       //Make sure we are large enough to at least hold the metadata. If not, return false
       else if(blockCount * blockSize < sizeof( BaseContainerMetadata)) {
          std::cerr << "BaseContainer instantiation failed. Size not large enough for metadata" << std::endl;
          return false;
       } 
       else {
          m_blockCount = blockCount;
          m_blockSize  = blockSize;
       }

       //Allocate enough memory for the met
       if( !m_buffer.allocate( m_blockCount * m_blockSize )) {
          std::cerr << "BaseContainer: Failed to allocated "<<m_blockCount * m_blockSize<<" bytes."<<std::endl;
          return false;
       }

       //Map metadat to the appropriate pointer
       m_metadata = (BaseContainerMetadata *)(&m_buffer[0]);
       m_metadata->m_offset = sizeof( BaseContainerMetadata);
       m_metadata->m_type   = TYPE_BASE;

       m_metadata->m_size = m_blockCount * m_blockSize;

       return true;
    }
    
   /**
    * \brief Function to save the base container
    **/
   bool BaseContainer::save( std::string filename ) 
   {
      bool rc = true;
      std::string header = m_metadata->getJsonString();

      //Open the specified filename
      int fd = open( filename.c_str(), O_WRONLY |O_CREAT, 0777 );
      if( fd < 0 ) {
         std::cerr << "BaseContainer unable to open "<<filename.c_str()<<std::endl;
         return false;
      }


      //Write the header
      size_t byteCount = 0;
      ssize_t result = 0;
      while((result >= 0 )&&( byteCount < m_blockCount * m_blockSize )) {
         result = write( fd, m_metadata, m_blockCount * m_blockSize - byteCount ); 
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
      container.allocate(100);

      sz = container.getSize();
      if( sz != bcSize ) {
         std::cout << "Invalid container size2: "<<sz << "!="<<bcSize<<std::endl;
         return false;
      }

      container.m_metadata->m_id = 1;
      container.m_metadata->m_offset = 2;

      std::string expected = "{\"id\":1,\"size\":100,\"offset\":2,\"type\":1}";
      std::string result   = container.m_metadata->getJsonString();

      if( result.compare(expected)) {
         std::cerr << "testBaseContainer failed:"<<result.c_str()<<"!="<<expected.c_str()<<std::endl;
         return false;
      }

      return true;
 
   }
};
