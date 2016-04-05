#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>

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
    * \return number of bytes in the data buffer, 0 on error
    *
    * This function allocates an image object
    **/
   size_t ImageContainer::allocate( uint32_t width
                                  , uint32_t height
                                  , uint32_t bpp
                                  , uint32_t mode
                                  , size_t   sz
                                  )
   {
      //Calculate the size if it is not specified
      if( sz == 0 ) { 
         sz = width * height *bpp / 8;
      }

      size_t result = BaseContainer::allocate( sz );
      if( result == 0 ) {
         std::cerr << "ImageContainer allocation failed!"<<std::endl;
      } 
      else {
         m_metadata = static_cast<ImageMetadata *>(BaseContainer::m_metadata);
         m_metadata->m_width  = width;
         m_metadata->m_height = height;
         m_metadata->m_bpp    = bpp;
         m_metadata->m_mode   = mode;
      }

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
   size_t ImageContainer::allocate( ImageMetadata metadata, size_t sz ) 
   {
      return allocate( metadata.m_width
                     , metadata.m_height
                     , metadata.m_bpp
                     , metadata.m_mode
                     , sz
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
    * \brief test function for the BaseMetadata class
    * \return true on success, false on failure
    **/
   bool testImageContainer()
   {
      ImageMetadata metadata;
      metadata.m_width = 100;
      metadata.m_height = 100;
      metadata.m_bpp = 8;
      metadata.m_mode = APL_MODE_GRBG;

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

      container1.save("test1.tmp");
      container2.save("test2.tmp");

      bool rc = true;
      int info = std::system("diff test1.tmp test2.tmp > test.tmp");
      info = std::ifstream("test.tmp").get();
      if( info != -1 ) {
         rc = false;
      }

      info = std::system("rm *.tmp");

      return rc;
   }
};
