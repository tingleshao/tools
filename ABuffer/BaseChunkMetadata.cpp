#include <iostream>
#include <sstream>
#include <string>

#include "BaseChunkMetadata.h"

namespace atl
{
   /**
    * \brief Generates a jsonString based on the contained metadata
    * \param [in] brackets flag to indicate if surrounding brackets are needed (default = true)
    * \return std::string with Json representation
    **/
   std::string BaseChunkMetadata::getJsonString( bool brackets)
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
    * \brief Returns the size of the data contained in the metadata
    *
    * This function must be implemented by all metadata classes to ensure that 
    * the appropriate size is returned.
    **/
   size_t BaseChunkMetadata::getSize() 
   {
      return BASECONTAINERMETA_SIZE + m_type.length();
      
   }
   /**
    * \brief test function for the BaseChunkMetadata class
    * \return true on success, false on failure
    **/
   bool testBaseChunkMetadata()
   {
      BaseChunkMetadata metadata;
      metadata.m_id = 1234;
      metadata.m_offset = 1235;
      metadata.m_elementSize = 1;

      std::string expected("{\"id\":1234,\"offset\":1235,\"elementSize\":1}");
      std::string result = metadata.getJsonString();

      if( expected.compare(result)) {
         std::cout << "testBaseChunkMetadata failed(true). "<<result<<"!="<<expected<<std::endl;
         return false;
      }

      expected.assign("\"id\":1234,\"offset\":1235,\"elementSize\":1");
      result.clear();
      result = metadata.getJsonString(false);
      if( expected.compare(result)) {
         std::cout << "testBaseChunkMetadata failed(false). "<<result<<"!="<<expected<<std::endl;
         return false;
      }

      return true;
   }
};
