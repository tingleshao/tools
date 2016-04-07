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
   /**
    * \brief This class is a very simple container class that represents image data.
    **/
   class ImageContainer : public BaseContainer
   {
      private:
      protected:

      public: 
         size_t getPixelDataSize();
         std::string getJsonMetadata();
         size_t allocate( ImageMetadata metadata, size_t sz=BLK_SIZE_DEFAULT, size_t blockSize=BLK_SIZE_BYTE );
         size_t allocate( uint32_t width
                        , uint32_t height
                        , uint32_t bpp
                        , uint32_t mode
                        , size_t   sz = 0
                        , size_t   block_size = BLK_SIZE_BYTE
                        );


         //Access functions
         bool          setMetadata( ImageMetadata metadata );
         ImageMetadata getMetadata();
         uint32_t getWidth();
         uint32_t getHeight();
         uint32_t getBpp();
         uint32_t getMode();

         //Storage Functions
         bool savePNM( std::string filename, std::string metadata );
   };

   ImageContainer genTestImage(size_t width=256, size_t height=256 );
   bool testImageContainer();
};


