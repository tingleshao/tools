#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>

#include <fcntl.h>
#include <unistd.h>

#include "Timer.h"
#include "BaseContainer.h"

namespace atl
{
   /**
    * \brief allocates container data
    * \param [in] bytes      number of bytes data is expected to require
    * \param [in] blockSize  minimum size of a block in bytes (default = 1)
    * \param [in] metaSize   size of the metadata header. 0 indicates BaseMetadataSize. (default = 0)
    * \return number of bytes avialable in the data buffer (0 indicates failure)
    *
    * This function allocates the buffer to the given size and assigns the appropriate ID.
    * To speed up file I/O and data management, the memory is stored as a continuous array 
    * with that is an integer multiple of the blockSize as specified by the block count. The 
    * first bytes in the memory contain the metadata pointer. 
    **/
    size_t BaseContainer::allocate( size_t bytes, size_t blockSize, size_t metaSize ) 
    {
       if( blockSize == BLK_SIZE_DEFAULT ) {
          blockSize = m_blockSize;
       }
       else {
          m_blockSize = blockSize;
       }


       //If metadata is undefined, set it to the basecontainer size
       if( metaSize == 0 ) {
          metaSize = sizeof( BaseContainerMetadata );
       }

       size_t totalBytes = bytes + metaSize;

       m_blockCount = (totalBytes+m_blockSize-1)/m_blockSize;

       //Allocate enough memory for the met
       if( !m_buffer.allocate( m_blockCount * m_blockSize )) {
          std::cerr << "BaseContainer: Failed to allocated "<<m_blockCount * m_blockSize<<" bytes."<<std::endl;
          return 0;
       }

       //Map metadat to the appropriate pointer
       m_metadata = reinterpret_cast<BaseContainerMetadata *>(&m_buffer[0]);
       m_metadata->m_id = getTimeStamp();
       m_metadata->m_offset = metaSize;
       m_metadata->m_type   = TYPE_BASE;
       m_metadata->m_size = m_blockCount * m_blockSize;
       std::strncpy( m_metadata->m_magic, "ATLc\n", MAGIC_SIZE);

       return m_metadata->m_size - m_metadata->m_offset;
    }

   /**
    * \brief sets the container ID to the specified value
    **/ 
    void BaseContainer::setId( uint64_t id ) 
    {
       m_metadata->m_id =id;
    }

   /**
    * \brief Function to save the base container
    * \param [in] filename name of the file to save the container to
    *
    * This functions saves the binary data in the container directly to disk.
    **/
   bool BaseContainer::save( std::string filename, size_t blockSize ) 
   {
      //If no block size is specified, use default
      if( blockSize == BLK_SIZE_DEFAULT ) {
         blockSize = m_blockSize;
      }

      //Open the specified filename
      int fd = open( filename.c_str(), O_WRONLY |O_CREAT, 0777 );
      if( fd < 0 ) {
         std::cerr << "BaseContainer unable to open "<<filename.c_str()<<std::endl;
         return false;
      }

  
      //Write data
      bool    rc = true;
      size_t  byteCount = 0;
      ssize_t result = 0;
      uint8_t * buff = (uint8_t *)&m_buffer[0];
//      while((result >= 0 )&&( byteCount < m_blockCount * m_blockSize )) {
      while((result >= 0 )&&( byteCount < m_metadata->m_size )) {
         result = write( fd, &buff[byteCount], m_metadata->m_size - byteCount ); 
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
    * \brief returns the size of the metadata
    *
   size_t BaseContainer::getMetadataSize()
   {
      return m_metadata->m_offset;
   }
   */
  
   /**
    * \brief Returns a pointer to the head of the data
    **/
   void * BaseContainer::getDataPointer()
   {
      return &m_buffer[0]+m_metadata->m_offset;
   }

   /**
    * \brief Returns the container ID
    **/
   uint64_t BaseContainer::getId()
   {
      if( m_metadata == NULL ) {
         return 0;
      }

      return m_metadata->m_id;

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

      std::string expected = "{\"id\":1,\"type\":1,\"size\":148,\"offset\":48}";
      std::string result   = container.m_metadata->getJsonString();

      if( result.compare(expected)) {
         std::cerr << "testBaseContainer failed:\n"
                   << "result   :"<<result.c_str()<<"\n"
                   << "expected :"<<expected.c_str()<<std::endl;
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

      /////////////////////////////////////////////
      // Check file I/O
      /////////////////////////////////////////////
      bool rc = true;
      container.save("cont1.tmp");

      BaseContainer container2;
      container2.allocate( bcSize, BLK_SIZE_4MB );
      //Check if we can save data
      container2.save("cont2.tmp");

      std::ifstream f1("cont1.tmp", std::ios::binary | std::ios::ate);
      if( (unsigned)f1.tellg() != bcSize + sizeof( BaseContainerMetadata )) {
         std::cerr << "BaseContainer1 unexpected file size: "
                   <<f1.tellg()<<"!="<<bcSize+sizeof(BaseContainerMetadata)
                   <<std::endl;
         rc = false;
      }

      std::ifstream f2("cont2.tmp", std::ios::binary | std::ios::ate);
      if( f2.tellg() != BLK_SIZE_4MB ) {
         std::cerr << "BaseContainer2 unexpected file size: "
                   <<f2.tellg()<<"!="<<BLK_SIZE_4MB
                   <<std::endl;
         rc = false;
      }
      /*
      int irc = std::system("rm *.tmp");
      if( irc != 0 ) {
         std::cerr << "Failed on system command with code "<<irc <<std::endl;
         rc = false;
      }
      */


      return rc;
 
   }
};
