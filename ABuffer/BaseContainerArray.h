#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <TSArray.h>

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
         BaseContainerArrayMetadata metadata;              //!< Metadata about this container
         TSArray<BaseContainer>     containerArray;        //!< Array of container objects
         
         //Interface functions
         size_t push(BaseContainer container);
         BaseContainer pop();
         BaseContainer operator[] (size_t index) const {return containerArray[index];};
   };

   //Test functions
   bool testBaseContainerArray();
};
