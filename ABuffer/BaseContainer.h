#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseContainerMetadata.h>
#include <BaseBuffer.h>

#define MAGIC "AGT"; //Aqueti Generic container

namespace atl
{
   /**
    * \brief Basic container class that associates metadata with a buffer
    **/
   class BaseContainer
   {
      private:
         
      public: 
         BaseContainerMetadata m_metadata;  //!< Metadata fro the container
         BaseBuffer   m_buffer;             //!< DataBuffer

         BaseContainer( uint64_t id = 0 );
         virtual ~BaseContainer();
         virtual bool allocate(size_t bytes = 0 );

         virtual bool save( std::string filename );
         uint64_t getId();
         size_t getSize();
   };

   //Test functions
   bool testBaseContainer();
};
