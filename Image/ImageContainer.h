#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <sstream>
#include <iostream>
#include <string>

#include <BaseContainer.h>
#include <ImageMetadata.h>

namespace atl
{
   /*
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
   const uint16_t APL_MODE_RGGB=10;     //!< Single color per pixel, first line R,G second G,B
   */
   /**
    * \brief This class is a very simple container class that represents image data.
    **/
   class ImageContainer : public BaseContainer
   {
      private:
      protected:
         ImageMetadata * m_metadata;

      public: 
         std::string getJsonMetadata();
         size_t allocate( ImageMetadata metadata, size_t sz=0 );
         size_t allocate( uint32_t width
                        , uint32_t height
                        , uint32_t bpp
                        , uint32_t mode
                        , size_t sz = 0
                        );


         //Access functions
         bool          setMetadata( ImageMetadata metadata );
         ImageMetadata getMetadata();
         uint32_t getWidth();
         uint32_t getHeight();
         uint32_t getBpp();
         uint32_t getMode();


   };

   bool testImageContainer();
};


