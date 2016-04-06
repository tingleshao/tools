#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseContainer.h>
#include <HContainerMetadata.h>
#include <BaseBuffer.h>


namespace atl
{
   #define BLK_SIZE_BYTE 1                      //!<4 megabyte block size in bytes
   #define BLK_SIZE_4MB  4194304                //!<4 megabyte block size in bytes

   /**
    * \brief Container class for a heterogenous mix of containers stored in common memory
    **/
   class HContainer : public BaseContainer
   {
      private:
         uint64_t * offsetArray = NULL;          //!< Pointer to the table of offsets in memory

      protected:
         
      public: 
         HContainerMetadata * m_metadata = NULL; //!< Metadata for the container

         HContainer();
         bool add( BaseContainer & container );
         bool getContainer( BaseContainer & container, uint64_t index );
//         virtual bool load( std::string filename );
   };

   //Test functions
   bool testHContainer();
};
