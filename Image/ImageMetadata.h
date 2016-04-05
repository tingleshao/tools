#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <BaseContainerMetadata.h>

namespace atl
{
   #define  TYPE_IMAGE 2

   // Image modes
   const uint16_t APL_MODE_NONE=0;      //!< No data yet stored in the image
   const uint16_t APL_MODE_GRAY=1;      //!< Single gray-scale value, non-Bayer
   const uint16_t APL_MODE_GRBG=2;      //!< Single color per pixel, first line G,R second B,G
   const uint16_t APL_MODE_BGGR=3;      //!< Single color per pixel, first line B,G second G,R
   const uint16_t APL_MODE_RGB=4;       //!< Three colors per pixel, in order R,G,B
   const uint16_t APL_MODE_BGR=5;       //!< Three colors per pixel, in order B,G,R
   const uint16_t APL_MODE_JPEG_RGB=6;  //!< Jpeg-compressed RGB image.
   const uint16_t APL_MODE_YUV_422=7;   //!< YCrCb in 422 format
   const uint16_t APL_MODE_BGRA=8;      //!< Four colors per pixel, in order B,G,R,A
   const uint16_t APL_MODE_BT601_YUV_422=9; //!< YCrCb in 422 format with black of 16 white of 235
   
   /**
    * \brief Metadata for a generic image object
    *
    * This class contains the metadata for an image object. It specifies the pixel format 
    * and extent. 
    **/
   class ImageMetadata : public BaseContainerMetadata
   {
      private:
         
      public: 
         //Image size and scale
         uint16_t m_mode   = 0;               //!< Image mode (Bayer, RGB, etc)
         uint16_t m_width  = 0;               //!< Width of image
         uint16_t m_height = 0;               //!< Height of image
         uint16_t m_bpp    = 0;               //!< Bits per pixel

         ImageMetadata();
         std::string getJsonString( bool brackets = true );
   };


   //Test functions
   bool testImageMetadata();
};
