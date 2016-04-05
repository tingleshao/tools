#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseContainerMetadata.h>
#include <BaseBuffer.h>


namespace atl
{
   #define BLK_SIZE_BYTE 1                      //!<4 megabyte block size in bytes
   #define BLK_SIZE_4MB  4194304                //!<4 megabyte block size in bytes

   /**
    * \brief Basic container class that associates metadata with a buffer
    **/
   class BaseContainer
   {
      private:
         size_t m_blockSize  = 0;                   //!< Minimum memory chunk in the container
         size_t m_blockCount = 0;                   //!< Number of blocks in the container. 0 is unlimited
         
      public: 
         BaseContainerMetadata * m_metadata = NULL;//!< Metadata fro the container
         BaseBuffer   m_buffer;                    //!< DataBuffer

         virtual size_t allocate( size_t bytes      = 0
                                , size_t blockSize  = BLK_SIZE_BYTE
                                );

         size_t       getDataSize();
         void *       getDataPointer();
         virtual bool save( std::string filename );
         uint64_t     getId();
         size_t       getSize();
   };

   //Test functions
   bool testBaseContainer();
};
