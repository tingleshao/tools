#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <Timer.h>
#include <ImageMetadata.h>

namespace atl
{
   /**
    * \brief structure that defines the timing information for an image
    **/
   struct FrameID 
   {
      uint16_t frameId = 0;
      uint32_t utc = atl::getTimeId();
   }

   union FrameUnion
   {
      uint64_t integer;
      FrameId  data;
   }

   /**
    * \brief Metadata for a frame that includes image and timing data
    *
    * This class is a very simple interface to represent a continuous array of data.
    * The size value is the number of elements in the buffer and the buffer is the data
    * itself. There is no inherent method for data management allocated data is must be
    * freed with an external call to the deallocate function
    **/
   class FrameMetadata : public ImageMetadata
   {
      private:
         
      public: 
         uint16_t

         std::string getJsonString( bool brackets = true );
   };


   //Test functions
   bool testImageMetadata();
};
