#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <random>
#include <iostream>

#include "Timer.h"
#include "BaseContainerMetadata.h"

namespace atl
{
   /**
    * \brief Constructor
    *
    * The constructor should automatically generate a unique when created
    **/ 
   BaseContainerMetadata::BaseContainerMetadata()
   {
      std::strncpy( m_magic, "ATLc\n", MAGIC_SIZE);
      m_id = getTimeStamp();
      m_metaSize = sizeof( BaseContainerMetadata);
      m_type     = TYPE_BASE;
   }

   /**
    * \brief Generates a jsonString based on the contained metadata
    * \param [in] brackets flag to indicate if surrounding brackets are needed (default = true)
    * \return std::string with Json representation
    **/
   std::string BaseContainerMetadata::getJsonString( bool brackets)
   {
      std::stringstream ss;

      if( brackets ) {
         ss << "{";
      }
      
      ss << "\"id\":"        << m_id        << ","
         << "\"type\":"      << m_type      << ","
         << "\"size\":"      << m_size;

      if(brackets) { 
         ss << "}";
      }

      std::string result = ss.str();
      return result;
   }

   /**
    * \brief Returns the size of the data contained in the metadata
    *
    * This function must be implemented by all metadata classes to ensure that 
    * the appropriate size is returned.
    **/
   size_t BaseContainerMetadata::getSize() 
   {
      return sizeof( BaseContainerMetadata);
      
   }
   /**
    * \brief test function for the BaseContainerMetadata class
    * \return true on success, false on failure
    **/
   bool testBaseContainerMetadata()
   {
      BaseContainerMetadata metadata;
      metadata.m_id     = 1234;
      metadata.m_offset = 1235;
      metadata.m_size   = 1;

      std::string expected("{\"id\":1234,\"type\":1,\"size\":1}");
      std::string result = metadata.getJsonString();

      if( expected.compare(result)) {
         std::cout << "testBaseContainerMetadata failed(true).\n"
                   << "result:   "<<result<<"\n"
                   << "expected: "<<expected<<std::endl;
         return false;
      }

      expected.assign("\"id\":1234,\"type\":1,\"size\":1");
      result = metadata.getJsonString(false);

      if( expected.compare(result)) {
         std::cout << "testBaseContainerMetadata failed(true).\n"
                   << "result:   "<<result<<"\n"
                   << "expected: "<<expected<<std::endl;
         return false;
      }

      return true;
   }
}
