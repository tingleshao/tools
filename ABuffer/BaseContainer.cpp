#include <BaseContainer.h>

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
   size_t BaseContainer::push_back( BaseChunk chunk)
   {
      m_metadata.m_elementCount = m_containerArray.push_back(chunk);
      m_metadata.m_size += chunk.getSize();

      return m_metadata.m_elementCount;
   }

   /** 
    * \brief Size of all of the data in the container
    **/
   size_t BaseContainer::getSize() 
   {
      return m_metadata.getSize();
   }

   //Test functions
   bool testBaseContainer() 
   {
      BaseContainer arr;
      BaseChunk chunk;
  
      size_t expected = BC_META_SIZE;
      size_t baseSize = arr.getSize();
      if( baseSize != expected ) {
         std::cout << "invalid intial array size:"<<baseSize<<"!="<<expected << std::endl;
         return false;
      }

      chunk.allocate(100);
      size_t itemSize = chunk.getSize();
      arr.push_back( chunk);
      size_t sz = arr.getSize();

      if( sz != itemSize+baseSize ) {
         std::cout << "Array size does not match chunk size:"<<sz<<"!="<<itemSize+baseSize<<std::endl;
         return false;
      }

      arr.push_back( chunk);
      sz = arr.getSize();
      if( sz != 2*itemSize+baseSize ) {
         std::cout << "Array size does not 2 chunk size:"<<sz<<"!="<<2*itemSize+baseSize<<std::endl;
         return false;
      }

      return true;
   }
};
