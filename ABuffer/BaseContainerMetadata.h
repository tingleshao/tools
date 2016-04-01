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
   class BaseContainerMetadata
   {
      private:
         
      public: 
         uint64_t    m_id = 0;              //!< ID of the object
         uint64_t    m_elementSize = 1;     //!< Size of a databuffer element
         uint64_t    m_elementCount = 0;    //!< Number of elements in the associated buffer
         std::string m_type;                //!< Indicates metadata type
         uint64_t    m_offset = 0;          //!< Offset into the binary data

         std::string getJsonString( bool brackets = true );
   };


   //Test functions
   bool testBaseContainerMetadata();
};
