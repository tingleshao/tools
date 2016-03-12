#include <iostream>
#include <sstream>
#include <string>

#include "BaseMetadata.h"

namespace atl
{
   /**
    * \brief Generates a jsonString based on the contained metadata
    *
    * \return std::string with Json representation
    **/
   std::string BaseMetadata::getJsonString()
   {
      std::stringstream ss;
      ss << "{"
         << "\"id\":" << m_id << ","
         << "\"offset\":" << m_offset 
         << "}";

      std::string result = ss.str();

      return result;
   }

   /**
    * \brief test function for the BaseMetadata class
    * \return true on success, false on failure
    **/
   bool testBaseMetadata()
   {
      BaseMetadata metadata;
      metadata.m_id = 1234;
      metadata.m_offset = 1235;

      std::string expected("{\"id\":1234,\"offset\":1235}");
      std::string result = metadata.getJsonString();

      if( !expected.compare(result)) {
      std::cout << "testBaseMetadata passed. "<<result<<"!="<<expected<<std::endl;
         return true;
      }

      std::cout << "testBaseMetadata failed. "<<result<<"!="<<expected<<std::endl;
      return false;
   }
};
