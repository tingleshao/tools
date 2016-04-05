#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <string>

namespace atl
{
   #define TYPE_SIZE 8
 
   #define TYPE_UNKNOWN 0
   #define TYPE_BASE  1

   /**
    * \brief Low level data structure to associate pointer with a data size
    *
    * This class is a very simple interface to represent a continuous array of data.
    * The size value is the number of elements in the buffer and the buffer is the data
    * itself. There is no inherent method for data management allocated data is must be
    * freed with an external call to the deallocate function
    **/
   class BaseContainerMetadata
   {
      private:
         
      public: 
         uint64_t    m_id = 0;              //!< ID of the object
         uint64_t    m_size = 0;            //!< Total size of the container including metadata
         uint64_t    m_offset = 0;          //!< Offset into the binary data
         uint64_t    m_type = TYPE_UNKNOWN; //!< Type of metadata

         BaseContainerMetadata();
         virtual size_t  getSize();         //!< Returns the size of the metadata container
         std::string getJsonString( bool brackets = true );
   };


   //Test functions
   bool testBaseContainerMetadata();
};
