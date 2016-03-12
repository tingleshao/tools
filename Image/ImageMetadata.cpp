#include <iostream>
#include <sstream>
#include <string>

#include "ImageMetadata.h"

namespace atl
{
   /**
    * \brief Generates a jsonString based on the contained metadata
    *
    * \return std::string with Json representation
    **/
   std::string ImageMetadata::getJsonString()
   {
      std::string baseString = BaseMetadata::getJsonString();
      baseString.pop_back();
      std::stringstream ss;

      ss << baseString << ","
         << "\"image\":{"
            << "\"mode\":"   << m_mode    << ","
            << "\"width\":"  << m_width   << ","
            << "\"height\":" << m_height  << ","
            << "\"bpp\":"  << m_bpp     
            << "}"
         << "}";

      std::string result = ss.str();

      return result;
   }

   /**
    * \brief test function for the BaseMetadata class
    * \return true on success, false on failure
    **/
   bool testImageMetadata()
   {
      ImageMetadata metadata;
      metadata.m_id = 1234;
      metadata.m_offset = 1235;
      metadata.m_mode = 1;
      metadata.m_width= 2;
      metadata.m_height= 3;
      metadata.m_bpp= 4;

      std::string expected("{\"id\":1234,\"type\":\"base\",\"offset\":1235,\"image\":{\"mode\":1,\"width\":2,\"height\":3,\"bpp\":4}}");
      std::string result = metadata.getJsonString();

      if( !expected.compare(result)) {
         return true;
      }

      std::cout << "testImageMetadata failed. "<<result<<"!="<<expected<<std::endl;
      return false;
   }
};
