#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseBuffer.h>

namespace atl
{
   struct ContainerMetadata
   {
      uint64_t timeStamp = 0;               //!< Time dataset was generated
      uint64_t id = 0;                      //!< ID of the dataset
   }


   /**
    * \brief Low level data structure to associate pointer with a data size
    *
    * This class is a very simple interface to represent a continuous array of data.
    * The size value is the number of elements in the buffer and the buffer is the data
    * itself. There is no inherent method for data management allocated data is must be
    * freed with an external call to the deallocate function
    **/
   class BaseContainer
   {
      private:
         
      public: 
         ContainerMetadata metadata;
         BaseBuffer buffer;


   };


   //Test functions
   bool testBaseBuffer();
};
