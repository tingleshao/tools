#include <BaseContainerArrayMetadata.h>

namespace atl
{
   /** 
    * \brief This function pushes a new container onto the end of the container array
    * \param [in] container object to be added to the array
    * \return number of elements in the array
    *
    * This function adds the specifed container to the end of the array and updates the 
    * cumulative size of the data contained in the array.
    **/ 
   size_t BaseContainerArray::push( BaseContainer container )
   {
   }

   {
      private:
         
      public: 
         BaseContainerArrayMetadata metadata;              //Metadata about this container
         TSArray<BaseContainer>     containerArray;        //Array of container objects
         
         //Interface functions
         size_t push(BaseContainer container);
         BaseContainer pop();
         BaseContainer operator[] (size_t index) const {return containerArray[index];};
   };

   //Test functions
   bool testBaseContainerArray();
};
