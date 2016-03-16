#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <string>

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
   class BaseMetadata
   {
      private:
         
      public: 
         uint64_t    m_id = 0;              //!< ID of the object
         uint64_t    m_offset = 0;          //!< Offset into binary data
         uint64_t    m_elementSize = 0;     //!< Size of a databuffer element
         std::string m_name;                //!< Indicates metadata type

         std::string getJsonString();
   };


   //Test functions
   bool testBaseMetadata();
};
