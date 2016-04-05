#include <BaseContainerArray.h>

namespace atl
{
   /** 
    * \brief This function pushes a new containerArray onto the end of the containerArray array
    * \param [in] containerArray object to be added to the array
    * \return number of elements in the array on success, UINT64_MAX on error
    *
    * This function adds the specifed containerArray to the end of the array and updates the 
    * cumulative size of the data contained in the array.
    **/ 
   size_t BaseContainerArray::push_back( BaseContainer container)
   {
      size_t result = UINT64_MAX;

      m_metadata.m_elementCount = m_containerArray.push_back(container);

      m_metadata.m_size += container.getSize();
      result = m_containerArray.getSize();

      return result;
   }

   /** 
    * \brief Size of all of the data in the containerArray
    **/
   size_t BaseContainerArray::getSize() 
   {
      return m_metadata.getSize();
   }

   /**
    * \brief Returns the number of elements in the containerArray
    **/
    size_t BaseContainerArray::getElementCount() 
    {
       return m_containerArray.getSize();
    }

   /**
    * \brief gets the ID of the containerArray
    * \return id of the containerArray
    **/ 
   size_t BaseContainerArray::getId()
   {
      return m_metadata.m_id;
   }

   /*
    * \brief Saves saves the containers into sequence of blocks
    * \param [in] filename name of the file to save to
    * \param [in] blockSize fundamental block size for the file
    *
    * This function maps off of the data into a contiguous block of memory on the file system
    *
   bool save( std::string filename, size_t blockSize )
   {
      //Create a container to hold all of the data
      std::vector<uint16_t> offsets(m_containerArray.getSize());
      ArrayContainer container;
      container.allocate( getSize()); 

      for( size_t i = 0; i < getElementCount(); i++ ) {
         

      }




      //Open the specified filename
      int fd = open( filename.c_str(), O_WRONLY |O_CREAT, 0777 );
      if( fd < 0 ) {
         std::cerr << "BaseContainer unable to open "<<filename.c_str()<<std::endl;
         return false;
      }


   }
   */
   //Test functions
   bool testBaseContainerArray() 
   {
      BaseContainerArray arr;
      BaseContainer      container;
  
      size_t expected = BC_META_SIZE;
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
