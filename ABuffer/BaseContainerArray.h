#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <TSArray.tcc>
#include <BaseContainer.h>
#include <BaseContainerArrayMetadata.h>

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
   class BaseContainerArray
   {
      private:
         
      public: 
         BaseContainerArrayMetadata m_metadata;            //!< Metadata about this container
         TSArray<BaseContainer>     m_containerArray;      //!< Array of container objects
         
         //Interface functions
//         BaseContainerArray::BaseContainerArray();
         size_t push_back(BaseContainer container);
         BaseContainer pop();
         BaseContainer operator[] (size_t index) const {return m_containerArray[index];};
         size_t getSize();
   };

   //Test functions
   bool testBaseContainerArray();
};
