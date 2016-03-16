#include <iostream>
#include <sstream>
#include <string>

#include "BaseContainer.h"

namespace atl
{
   /**
    * \brief allocates the container data.
    *
    * \param [in] bytes number of bytes to allocate (default = 0)
    * \return true on success, false on failure
    * 
    * This function allocates internal variables. All containers will contain a BaseMetadata 
    * and BaseBuffer class or derivatives. This function allocates the proper version based
    * on the class they are contained in.
    **/
   bool BaseContainer::create( size_t bytes) 
   {
      m_buffer   = new BaseBuffer;
      m_metadata = new BaseMetadata;

      return true;
   }

   BaseContainer::~BaseContainer()
   {
      //Delete the metadata
      if( m_metadata == NULL ) {
         delete m_metadata;
         m_metadata = NULL;
      }

      //Delete the buffer
      if( m_buffer != NULL ) {
         delete m_buffer;
         m_buffer = NULL;
      }
   }

   /**
    * \brief Test function
    **/
   bool testBaseContainer() 
   {
      BaseContainer container;
      container.create();

      container.m_metadata->m_id = 1;
      container.m_metadata->m_offset = 2;
      container.m_metadata->m_elementSize = 3;

      std::string expected = "{\"id\":1,\"offset\":2,\"elementSize\":3}";
      std::string result   = container.m_metadata->getJsonString();

      if( !result.compare(expected)) {
         return true;
      }

      std::cerr << "testBaseContainer failed:"<<result.c_str()<<"!="<<expected.c_str()<<std::endl;
      return false;
 
   }
};
