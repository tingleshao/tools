#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
#include <sstream>
#include <iostream>
#include <string>

#include <ImageMetadata.h>
#include <ExtendedBuffer.tcc>

namespace atl
{
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
   
   /**
    * \brief This class is a very simple container class that represents image data.
    **/
   template<typename T>
   class ImageContainer : public BaseContainer
   {
      private:
      protected:

      public: 
         ImageMetadata m_metadata;            //!<Image metadata structure
         ExtendedBuffer<T> m_data;            //!<Image Data Buffer
   };

   bool testImageContainer();
};


