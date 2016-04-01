#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <TSArray.tcc>
#include <BaseChunk.h>
#include <BaseContainerMetadata.h>

namespace atl
{
   /**
    * \brief Basic class that contains an array of containers.
    *
    * This this class manages multiple independent container objects, each
    * with their own metadata and allocated memory. Methods in the class
    * map the embedded containers into a aggregate container object with 
    * a common memory allocation
    **/
   class BaseContainer
   {
      private:
         
      public: 
         BaseContainerMetadata m_metadata;            //!< Metadata about this container
         TSArray<BaseChunk>     m_containerArray;      //!< Array of container objects
         
         //Interface functions
//         BaseContainer::BaseContainer();
         size_t push_back(BaseChunk chunk);
         BaseChunk pop();
         BaseChunk operator[] (size_t index) const {return m_containerArray[index];};
         size_t getSize();
   };

   //Test functions
   bool testBaseContainer();
};
