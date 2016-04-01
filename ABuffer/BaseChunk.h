#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseChunkMetadata.h>
#include <BaseBuffer.h>

#define MAGIC "AGT"; //Aqueti Generic container

namespace atl
{
   /**
    * \brief Basic container class that associates metadata with a buffer
    **/
   class BaseChunk
   {
      private:
         
      public: 
         BaseChunkMetadata m_metadata;  //!< Metadata fro the container
         BaseBuffer   m_buffer;             //!< DataBuffer

         BaseChunk( uint64_t id = 0 );
         virtual ~BaseChunk();
         virtual bool allocate(size_t bytes = 0 );

         virtual bool save( std::string filename );
         uint64_t getId();
         size_t getSize();
   };

   //Test functions
   bool testBaseChunk();
};
