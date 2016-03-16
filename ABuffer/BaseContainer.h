#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseMetadata.h>
#include <BaseBuffer.h>

namespace atl
{
   /**
    * \brief Basic container class that associates metadata with a buffer
    **/
   class BaseContainer
   {
      private:
         
      public: 
         BaseMetadata * m_metadata = NULL;  //!< Metadata fro the container
         BaseBuffer   * m_buffer   = NULL;  //!< DataBuffer

         virtual ~BaseContainer();
         virtual bool create(size_t bytes = 0 );
   };

   //Test functions
   bool testBaseContainer();
};
