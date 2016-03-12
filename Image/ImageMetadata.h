#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseMetadata.h>

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
   class ImageMetadata : public BaseMetadata
   {
      private:
         
      public: 
         uint16_t m_mode   = 0;               //!< Image mode (Bayer, RGB, etc)
         uint16_t m_width  = 0;               //!< Width of image
         uint16_t m_height = 0;               //!< Height of image
         uint16_t m_bpp    = 0;               //!< Bits per pixel


         std::string getJsonString();
   
   };


   //Test functions
   bool testImageMetadata();
};
