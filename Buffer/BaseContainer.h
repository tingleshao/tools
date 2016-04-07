#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseContainerMetadata.h>
#include <BaseBuffer.h>


namespace atl
{
   #define BLK_SIZE_DEFAULT 0                   //!< Use Default block size
   #define BLK_SIZE_BYTE 1                      //!< 1 byte block size
   #define BLK_SIZE_4MB  4194304                //!<4 megabyte block size in bytes

   /**
    * \brief Basic container class that associates metadata with a buffer
    **/
   class BaseContainer
   {
      private:

      protected:
         
      public: 
         size_t m_blockSize  = BLK_SIZE_BYTE;                   //!< Minimum memory chunk in the container
         size_t m_blockCount = 0;                   //!< Number of blocks in the container. 0 is unlimited

         BaseContainerMetadata * m_metadata = NULL; //!< Metadata fro the container
         BaseBuffer   m_buffer;                     //!< DataBuffer

         virtual size_t allocate( size_t bytes      
                                , size_t blockSize  = 0
                                , size_t metaSize   = 0
                                );

         size_t       getDataSize();
         void *       getDataPointer();
         bool         save( std::string filename, size_t blockSize = BLK_SIZE_DEFAULT );
         void         setId( uint64_t id );
         uint64_t     getId();
         size_t       getSize();
   };

   //Test functions
   bool testBaseContainer();
};
