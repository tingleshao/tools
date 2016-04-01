#include <iostream>
#include <sstream>
#include <string>

#include "BaseContainer.h"

namespace atl
{
   /**
    * \brief constructor
    **/
    BaseContainer::BaseContainer( uint64_t id ) 
    {
      m_metadata.m_id = id;
    }
    
   /**
    * \brief Destructore
    **/
   BaseContainer::~BaseContainer( )
   {
   }
   /**
    * \brief allocates the container data.
    *
    * \param [in] bytes number of bytes to allocate (default = 0)
    * \return true on success, false on failure
    * 
    * This function allocates internal variables. All containers will contain a BaseContainerMetadata 
    * and BaseBuffer class or derivatives. This function allocates the proper version based
    * on the class they are contained in.
    **/
   bool BaseContainer::allocate( size_t bytes) 
   {
      if( !m_buffer.allocate( bytes )) {
         std::cerr << "BaseContainer: Failed to allocated "<<bytes<<" bytes."<<std::endl;
         return false;
      }

      return true;
   }


   /**
    * \brief Function to save the base container
    **/
   bool BaseContainer::save( std::string filename ) 
   {
      std::cerr << "saveBaseContainer not currently supported" <<std::endl;
      return false;
   }

   /**
    * \brief returns the size of the container
    **/
    size_t BaseContainer::getSize() 
    {
       return m_metadata.getSize() + m_buffer.getSize();
    }

   /**
    * \brief Test function
    **/
   bool testBaseContainer() 
   {
      size_t bcSize = BASECONTAINERMETA_SIZE;
      BaseContainer container;

      size_t sz = container.getSize();
      if( sz != bcSize ) {
         std::cout << "Invalid container size: "<<sz << "!="<<bcSize<<std::endl;
         return false;
      }
      container.allocate(100);

      sz = container.getSize();
      if( sz != bcSize + 100  ) {
         std::cout << "Invalid container size: "<<sz << "!="<<100+bcSize<<std::endl;
         return false;
      }

      container.m_metadata.m_id = 1;
      container.m_metadata.m_offset = 2;

      std::string expected = "{\"id\":1,\"offset\":2,\"elementSize\":1}";
      std::string result   = container.m_metadata.getJsonString();

      if( result.compare(expected)) {
         std::cerr << "testBaseContainer failed:"<<result.c_str()<<"!="<<expected.c_str()<<std::endl;
         return false;
      }

      return true;
 
   }
};
