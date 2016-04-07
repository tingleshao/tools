#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#include "ImageContainer.h"

namespace atl
{
   /**
    * \brief Function to allocate data based on the input information
    * \param [in] width width of the image
    * \param [in] height width of the image
    * \param [in] bpp number of bits per pixel 
    * \param [in] mode pixel format (RGB, GGBR, etc);
    * \param [in] sz size of the data buffer (0 auto calculates)
    * \param [in] blockSize minimum block size in the data structure
    * \return number of bytes in the data buffer, 0 on error
    *
    * This function allocates an image object
    **/
   size_t ImageContainer::allocate( uint32_t width
                                  , uint32_t height
                                  , uint32_t bpp
                                  , uint32_t mode
                                  , size_t   sz
                                  , size_t   blockSize
                                  )
   {
      //Calculate the size if it is not specified
      if( sz == 0 ) { 
         sz = width * height *bpp / 8;
      } 


      size_t result = BaseContainer::allocate( sz, blockSize, sizeof(ImageMetadata));
      if( result == 0 ) {
         std::cerr << "ImageContainer allocation failed!"<<std::endl;
         return result;
      } 

      m_metadata = reinterpret_cast<ImageMetadata *>(&m_buffer[0]);
      m_metadata->m_width         = width;
      m_metadata->m_height        = height;
      m_metadata->m_bpp           = bpp;
      m_metadata->m_mode          = mode;
      m_metadata->m_pixelDataSize = sz;

      return result;
   }

   /**
    * \brief Allocates a container based on the passed in metadata
    * \param [in] metadata ImageMetadata structure defining the image
    * \param [in] sz size of the data buffer (0 auto calculates)
    * \return number of bytes in the data buffer, 0 on error
    *
    * This function maps the parameters in the metadata to the allocate 
    * by values
    **/
   size_t ImageContainer::allocate( ImageMetadata metadata, size_t sz, size_t blockSize ) 
   {
      return allocate( metadata.m_width
                     , metadata.m_height
                     , metadata.m_bpp
                     , metadata.m_mode
                     , sz
                     , blockSize 
                     );
   }

   /**
    * \brief Generates a jsonString based on the contained metadata
    * \param [in] brackets flag to indicate if the output includes enclosing bracketss
    *
    * \return std::string with Json representation
    **/
   std::string ImageContainer::getJsonMetadata()
   {
      return m_metadata->getJsonString();
   }

   /**
    * \brief Returns the image width
    **/
   uint32_t ImageContainer::getWidth()
   {
      return m_metadata->m_width;
   }

   /**
    * \brief Returns the image width
    **/
   uint32_t ImageContainer::getHeight()
   {
      return m_metadata->m_height;
   }

   /**
    * \brief Returns the image width
    **/
   uint32_t ImageContainer::getBpp()
   {
      return m_metadata->m_bpp;
   }

   /**
    * \brief Returns the image width
    **/
   uint32_t ImageContainer::getMode()
   {
      return m_metadata->m_mode;
   }

   /**
    * \brief returns the ImageMetadata structure
    **/
   ImageMetadata ImageContainer::getMetadata() 
   {
      ImageMetadata metadata;
      memcpy( &metadata, m_metadata, sizeof( ImageMetadata ));
      return metadata;
   }

   /**
    * \brief Sets the metadata from the given structure
    **/
   bool ImageContainer::setMetadata( ImageMetadata metadata )
   {
      memcpy( m_metadata, &metadata, sizeof( ImageMetadata));
      return true;
   }

   /**
    * \brief returns the number of pixels actually in memory 
    *
    * This is not the same as the buffer size since the buffer may be 
    * overallocated to force a block boundary
    **/
   size_t ImageContainer::getPixelDataSize() {
      return m_metadata->m_pixelDataSize;
   }
  
   /**
    * \brief Saves the data as a PNM file
    * \param [in] filename name of the file to write to
    * \return true on success, false on failure
    *
    * This function generated a PNM file from the image data if it is
    * in an uncompressed format. This works well for Bayer data, RGB data, and
    * grayscale images. All metadata is stored as a JSON string in the comment 
    * line of the PNM header. When saving as a PNM file, block size is ignored, so
    * this storage method is primarily intended for debugging and analysis.
    **/
   bool ImageContainer::savePNM( std::string filename, std::string metadata )
   {
      //Make sure we are a valid object 
      if( m_metadata == NULL ) {
         std::cerr << "Unable to save PNM with undefined metadata" <<std::endl;
         return false;
      }
      uint8_t * buffer = static_cast<uint8_t *>(getDataPointer());
      if( buffer == NULL ) {
         std::cerr << "Unable to save PNM. Invalid data pointer received!" <<std::endl;
         return false;
      }

      if( m_metadata->m_pixelDataSize == 0 ) {
         std::cerr << "Unable to save PNM. No pixel data defined!" <<std::endl;
         return false;
      }

      //Sanity check to make sure we have bytes that make sense
      size_t dataSize = getDataSize();
      if( dataSize < getPixelDataSize()) {
         std::cerr << "Unable to save PNM. dataSize less than defined pixel data\n"
                   << dataSize << " < "<< getPixelDataSize() <<std::endl;
         return false;
      }

      int bpp = m_metadata->m_bpp;
      bpp = pow(2,bpp)-1;
      std::stringstream ss;

      //Determine header information
      if(( m_metadata->m_mode == ATL_MODE_GRAY ) ||
         ( m_metadata->m_mode == ATL_MODE_GRBG ) ||
         ( m_metadata->m_mode == ATL_MODE_BGGR ))
      {
         ss<<"P5\n";
      }
      else if(( m_metadata->m_mode == ATL_MODE_RGB ) ||
              ( m_metadata->m_mode == ATL_MODE_BGR ))
      {
         ss<<"P6\n";
      }
      else  {
         std::cerr << "Unable to save PNM for mode "<<m_metadata->m_mode<<std::endl;
      }

      ss << "#" << metadata.c_str() << "\n";
      ss << m_metadata->m_width<<" "<<m_metadata->m_height<<"\n";
      ss << bpp <<"\n";
      std::string header = ss.str();

      //Open the file
      int fd = open( filename.c_str(), O_WRONLY | O_CREAT, 0777 );
      if( fd < 0 ) {
         std::cerr << "BaseContainer unable to open "<<filename.c_str()<<std::endl;
         return false;
      }

      //Write the header
      ssize_t result = write( fd, header.c_str(), header.length());
      if( result < 0 ) {
         std::cout << "Unable to write PNM header. Failure!"<<std::endl;
         return false;
      }

      //Loop to write data
      size_t byteCount = 0;
      while(( result >= 0 )&&(byteCount < m_metadata->m_pixelDataSize)) 
      {
         result = write( fd, buffer+byteCount, m_metadata->m_pixelDataSize - byteCount );
         if( result > 0 ) {
            byteCount += result;
         }
      }

      if( result < 0 ) {
         std::cerr << "Failed to write PNM. Exiting\n"<<std::endl;
         return false;
      }

      return true;
   }

   /**
    * \brief Generates a test image
    * \param [in] width width of the image
    * \param [in] height height of the image
    * \return test image
    **/
   ImageContainer genTestImage( size_t width, size_t height )
   {
      ImageMetadata metadata;
      metadata.m_width =  width;
      metadata.m_height = height;
      metadata.m_bpp = 8;
      metadata.m_mode = ATL_MODE_GRAY;

      ImageContainer pnm;
       
      if( !pnm.allocate( metadata)) {
         std::cerr<<"Failed to allocate"<<std::endl;
      }
       
      uint8_t * buffer = reinterpret_cast<uint8_t *>(pnm.getDataPointer());
      for( uint32_t i = 0; i < width; i++ ) 
      {
         for( uint32_t j = 0; j < height; j++ ) 
         buffer[i*height+j] = i % 255;
      }

      return pnm;
   }

   /**
    * \brief test function for the BaseMetadata class
    * \return true on success, false on failure
    **/
   bool testImageContainer()
   {
      size_t width = 256, height=256;

      ImageMetadata metadata;
      metadata.m_width =  width;
      metadata.m_height = height;
      metadata.m_bpp = 8;
      metadata.m_mode = ATL_MODE_GRBG;


      ImageContainer container1;
      ImageContainer container2;
      container1.allocate( metadata.m_width
                        , metadata.m_height
                        , metadata.m_bpp
                        , metadata.m_mode 
                        );

      container2.allocate( metadata );

      container1.setId(1);
      container2.setId(2);

      if( container1.getId() != container2.getId()-1) {
         std::cout << "Unable to set Ids:"<<container1.getId()<<"!="<<container2.getId()<<std::endl;
         return false;
      }

      container2.setId(1);
      ImageMetadata meta1 = container1.getMetadata();
      ImageMetadata meta2 = container2.getMetadata();
      if(( meta1.m_id     != meta2.m_id     )||
         ( meta1.m_size   != meta2.m_size   )||
         ( meta1.m_offset != meta2.m_offset )||
         ( meta1.m_width  != meta2.m_width  )||
         ( meta1.m_height != meta2.m_height )||
         ( meta1.m_bpp    != meta2.m_bpp    )||
         ( meta1.m_mode   != meta2.m_mode   )
        )
      {
         std::cout << "Metadata values do not match!"<<std::endl;
         return false;
      }

      uint8_t * buffer1 = (uint8_t *)container1.getDataPointer();
      uint8_t * buffer2 = (uint8_t *)container2.getDataPointer();

      for( uint32_t i = 0; i < container1.getDataSize(); i++ )
      {
         buffer1[i] = i %255;
         buffer2[i] = i %255;
      }

      std::string json1 = container1.getJsonMetadata();
      std::string json2 = container2.getJsonMetadata();

      if( json1.compare(json2)) {
         std::cout<<"JSON mismatch:\n" 
                  <<"json1: "<<json1.c_str() << "\n"
                  <<"json2: "<<json2.c_str() <<std::endl;

         return false;
      }


      /////////////////////////////////////////////
      // File system testing
      /////////////////////////////////////////////
      int info = std::system("mkdir test_data");

      container1.save("test_data/test1.tmp");
      container2.save("test_data/test2.tmp");

      bool rc = true;
      info = std::system("diff test_data/test1.tmp test_data/test2.tmp > test/test.tmp");
      info = std::ifstream("test_data/test.tmp").get();
      if( info != -1 ) {
         rc = false;
      }

      ImageContainer container3;
      metadata.m_mode = ATL_MODE_JPEG_RGB;
      container3.allocate( metadata, 100, BLK_SIZE_4MB );

      container3.save( "test_data/test3.tmp");

      std::ifstream f2("test_data/test3.tmp", std::ios::binary | std::ios::ate);
      if( f2.tellg() != BLK_SIZE_4MB ) {
         std::cerr << "BaseContainer2 unexpected file size: "
                   <<f2.tellg()<<"!="<<BLK_SIZE_4MB
                   <<std::endl;
         rc = false;
      }

       ImageContainer pnm = genTestImage();


       rc = pnm.savePNM("test.pnm", pnm.getJsonMetadata());
         

      info = std::system("rm test_data/*.tmp");
      info = std::system("rmdir test_data");

      return rc;
   }
};
