#include <BaseContainer.h>
#include <BaseContainerArray.h>

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
   size_t BaseContainerArray::push_back( BaseContainer container )
   {
      m_metadata.m_elementCount = m_containerArray.push_back(container);
      m_metadata.m_size += container.getSize();

      return m_metadata.m_elementCount;
   }

   /** 
    * \brief Size of all of the data in the container
    **/
   size_t BaseContainerArray::getSize() 
   {
      return m_metadata.getSize();
   }

   //Test functions
   bool testBaseContainerArray() 
   {
      BaseContainerArray arr;
      BaseContainer container;
  
      size_t expected = BC_ARRAY_META_SIZE;
      size_t baseSize = arr.getSize();
      if( baseSize != expected ) {
         std::cout << "invalid intial array size:"<<baseSize<<"!="<<expected << std::endl;
         return false;
      }

      container.allocate(100);
      size_t itemSize = container.getSize();
      arr.push_back( container);
      size_t sz = arr.getSize();

      if( sz != itemSize+baseSize ) {
         std::cout << "Array size does not match container size:"<<sz<<"!="<<itemSize+baseSize<<std::endl;
         return false;
      }

      arr.push_back( container);
      sz = arr.getSize();
      if( sz != 2*itemSize+baseSize ) {
         std::cout << "Array size does not 2 container size:"<<sz<<"!="<<2*itemSize+baseSize<<std::endl;
         return false;
      }

      return true;
   }
};
