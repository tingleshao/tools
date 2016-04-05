#include <iostream>
#include <sstream>
#include <string>
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
      m_id = getTimeStamp();
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
      
      ss << "\"id\":"     << m_id     << ","
         << "\"size\":"   << m_size   << ","
         << "\"offset\":" << m_offset << ","
         << "\"type\":"   << m_type;

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

      std::string expected("{\"id\":1234,\"size\":1,\"offset\":1235,\"type\":0}");
      std::string result = metadata.getJsonString();

      if( expected.compare(result)) {
         std::cout << "testBaseContainerMetadata failed(true). "<<result<<"!="<<expected<<std::endl;
         return false;
      }

      expected.assign("\"id\":1234,\"size\":1,\"offset\":1235,\"type\":0");
      result.clear();
      result = metadata.getJsonString(false);
      if( expected.compare(result)) {
         std::cout << "testBaseContainerMetadata failed(false). "<<result<<"!="<<expected<<std::endl;
         return false;
      }

      return true;
   }
}
