#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <string>

namespace atl
{
   #define BC_META_SIZE 24
   /**
    * \brief Low level data structure to associate pointer with a data size
    *
    * This class is a very simple interface to represent a continuous array of data.
    * The size value is the number of elements in the buffer and the buffer is the data
    * itself. There is no inherent method for data management allocated data is must be
    * freed with an external call to the deallocate function
    *
    * The m_size variable tracks the size of all data not inherent in the structure
    **/
   class BaseContainerMetadata
   {
      private:
         
      public: 
         uint64_t    m_id = 0;              //!< ID of the object
         uint64_t    m_elementCount = 0;    //!< Number of containers
         uint64_t    m_size = 0;            //!< size of containers (does not include header)

         size_t      getSize();             //!< Returns the size of the metadata

         std::string getJsonString( bool brackets = true );
   };

   //Test functions
   bool testBaseContainerMetadata();
};
