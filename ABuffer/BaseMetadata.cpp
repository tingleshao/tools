#include <iostream>
#include <sstream>
#include <string>

#include "BaseMetadata.h"

namespace atl
{
   /**
    * \brief Generates a jsonString based on the contained metadata
    * \param [in] brackets flag to indicate if surrounding brackets are needed (default = true)
    * \return std::string with Json representation
    **/
   std::string BaseMetadata::getJsonString( bool brackets)
   {
      std::stringstream ss;

      if( brackets ) {
         ss << "{";
      }
      
      ss << "\"id\":"     << m_id << ","
         << "\"offset\":" << m_offset << ","
         << "\"elementSize\":" << m_elementSize;

      if(brackets) { 
         ss << "}";
      }

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
      metadata.m_elementSize = 1;

      std::string expected("{\"id\":1234,\"offset\":1235,\"elementSize\":1}");
      std::string result = metadata.getJsonString();

      if( expected.compare(result)) {
         std::cout << "testBaseMetadata failed(true). "<<result<<"!="<<expected<<std::endl;
         return false;
      }

      expected.assign("\"id\":1234,\"offset\":1235,\"elementSize\":1");
      result.clear();
      result = metadata.getJsonString(false);
      if( expected.compare(result)) {
         std::cout << "testBaseMetadata failed(false). "<<result<<"!="<<expected<<std::endl;
         return false;
      }

      return true;
   }
};
