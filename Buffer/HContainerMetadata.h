#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <string>

#include <BaseContainerMetadata.h>

namespace atl
{
   const uint16_t DEFAULT_TABLE_SIZE=256;        //!< Default size of elements in lookup table
   /**
    * \brief Defines the Metadata structure for an HContainer class.
    *
    * This class provides access to an array of BaseContainerMetadata objects that
    * are stored in contiguous memory. 
    **/
   class HContainerMetadata : public BaseContainerMetadata
   {
      private:

      public: 
         uint16_t   m_usedBytes      = 0;                  //!< Number of bytes used in the container
         uint16_t   m_containerCount = 0;                  //!< Number of containers 
         uint16_t   m_tableSize      = DEFAULT_TABLE_SIZE; //!< Max number of containers(element in LUT)

         HContainerMetadata();
         size_t      getSize();
         std::string getJsonString( bool brackets = true );
   };


   //Test functions
   bool testHContainerMetadata();
}
