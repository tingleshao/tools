#include <iostream>
#include <sstream>
#include <string>
#include <BaseContainerArrayMetadata.h>

namespace atl
{
   /**
    * \brief Low level data structure to associate pointer with a data size
    *
    * This class is a very simple interface to represent a continuous array of data.
    * The size value is the number of elements in the buffer and the buffer is the data
    * itself. There is no inherent method for data management allocated data is must be
    * freed with an external call to the deallocate function
    **/
   std::string BaseContainerArrayMetadata::getJsonString( bool brackets ) 
   {
      std::stringstream ss;
      if( brackets ) {
         ss << "{";
      }

      ss << "\"id\":"           << m_id           << ","
         << "\"elementCount\":" << m_elementCount << ","
         << "\"size\":"         << m_size;

      if(brackets) {
         ss << "}";
      }

      std::string result = ss.str();
      return result;
   }


   //Test functions
   bool testBaseContainerArrayMetadata()
   {
      BaseContainerArrayMetadata meta;
      meta.m_id = 1;
      meta.m_elementCount = 2;
      meta.m_size = 3;

      std::string bracket ="{\"id\":1,\"elementCount\":2,\"size\":3}";
      std::string nobracket ="\"id\":1,\"elementCount\":2,\"size\":3";

      std::string result = meta.getJsonString(true);
      if( result.compare(bracket)) {
         std::cout << "testBaseContainerArrayMetadata failed with brackets" <<std::endl;
         return false;
      }

      result = meta.getJsonString(false);
      if( result.compare(nobracket)) {
         std::cout << "testBaseContainerArrayMetadata failed with no brackets" <<std::endl;
         return false;
      }

      return true;
   }
};
