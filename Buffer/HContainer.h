#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseContainer.h>
#include <HContainerMetadata.h>
#include <BaseBuffer.h>


namespace atl
{
   /**
    * \brief Container class for a heterogenous mix of containers stored in common memory
    **/
   class HContainer : public BaseContainer
   {
      private:
         uint16_t * m_offsetArray = NULL;          //!< Pointer to the table of offsets in memory

      protected:
         
      public: 
         HContainerMetadata * m_metadata = NULL; //!< Metadata for the container

         HContainer();
         size_t allocate( size_t bytes
                        , uint16_t tableSize = DEFAULT_TABLE_SIZE
                        , uint64_t blockSize = BLK_SIZE_DEFAULT
                        );
         bool add( BaseContainer & container );
         bool getContainer( BaseContainer & container, uint64_t index );
//         virtual bool load( std::string filename );
   };

   //Test functions
   bool testHContainer();
};
