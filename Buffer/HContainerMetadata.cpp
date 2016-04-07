#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <random>
#include <iostream>

#include "Timer.h"
#include "HContainerMetadata.h"

namespace atl
{
   /**
    * \brief Generates a jsonString based on the contained metadata
    * \param [in] brackets flag to indicate if surrounding brackets are needed (default = true)
    * \return std::string with Json representation
    **/
   std::string HContainerMetadata::getJsonString( bool brackets)
   {
      std::stringstream ss;

      if( brackets ) {
         ss << "{";
         ss << BaseContainerMetadata::getJsonString( false ) <<",";
      }

         ss << "\"objectData\":{"  
            << "\"containerCount\":" << m_containerCount << ","
            << "\"tableSize\":"  << m_tableSize  << ","
            << "\"usedBytes\":"      << m_usedBytes
            << "}";

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
   size_t HContainerMetadata::getSize() 
   {
      return sizeof( HContainerMetadata) + sizeof( uint64_t)*m_tableSize;
   }
   /**
    * \brief test function for the BaseContainerMetadata class
    * \return true on success, false on failure
    **/
   bool testHContainerMetadata()
   {
      HContainerMetadata metadata;
      metadata.m_id     = 1234;
      metadata.m_offset = 1235;
      metadata.m_size   = 1;
      metadata.m_containerCount = 4;
      metadata.m_tableSize  = 5;
      metadata.m_usedBytes      = 6;

      std::string expected("{\"id\":1234,\"type\":0,\"size\":1,\"offset\":1235,\"objectData\":{\"containerCount\":4,\"tableSize\":5,\"usedBytes\":6}}");
      std::string result = metadata.getJsonString();

      if( expected.compare(result)) {
         std::cout << "testBaseContainerMetadata failed(true).\n"
                   << "result:  " << result<<"\n"
                   << "expected:" << expected<<std::endl;
         return false;
      }

      expected.assign("\"objectData\":{\"containerCount\":4,\"tableSize\":5,\"usedBytes\":6}");
      result = metadata.getJsonString(false);
      if( expected.compare(result)) {
         std::cout << "testBaseContainerMetadata failed(true).\n"
                   << "result:  " << result<<"\n"
                   << "expected:" << expected<<std::endl;
         return false;
      }

      return true;
   }
}
