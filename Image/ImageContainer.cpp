#include <iostream>
#include <sstream>
#include <string>

#include "ImageContainer.h"

namespace atl
{
   /**
    * \brief Generates a jsonString based on the contained metadata
    * \param [in] brackets flag to indicate if the output includes enclosing bracketss
    *
    * \return std::string with Json representation
    **/
   std::string ImageContainer::getJsonString(bool brackets )
   {
      return m_metadata.getJsonString();
   }

   /**
    * \brief Returns the image width
    **/
   size_t ImageContainer::getWidth()
   {
      return m_metadata.m_width;
   }

   /**
    * \brief Returns the image width
    **/
   size_t ImageContainer::getHeight()
   {
      return m_metadata.m_height;
   }

   /**
    * \brief Returns the image width
    **/
   size_t ImageContainer::getBpp()
   {
      return m_metadata.m_bpp;
   }

   /**
    * \brief Returns the image width
    **/
   size_t ImageContainer::getMode()
   {
      return m_metadata.m_mode;
   }

   /**
    * \brief returns the ImageMetadata structure
    **/
   ImageMetadata ImageContainer::getMetadata() 
   {
      return m_metadata;
   }

   /**
    * \brief returns a jsonString of the metadat structure
    **/
   std::string ImageContainer::getMetadata() 
   {
      return m_metadata.getJsonString();
   }

   /**
    * \brief Sets the metadata from the given structure
    **/
   bool ImageContainer::setMetadata( ImageMetadata metadata )
   {
      m_metadata = metadata;
      return true;
   }

   /**
    * \brief Sets the width of the image
    **/
   void ImageContainer::setWidth( uint32_t width )
   {
      m_metadata.m_width = width;
   }
  
   /**
    * \brief Sets the width of the image
    **/
   void ImageContainer::setHeight( uint32_t width )
   {
      m_metadata.m_height = width;
   }
   /**
    * \brief Sets the width of the image
    **/
   void ImageContainer::setBpp( uint32_t width )
   {
      m_metadata.m_bpp = bpp;
   }
   /**
    * \brief Sets the width of the image
    **/
   void ImageContainer::setMode( uint32_t width )
   {
      m_metadata.m_mode = mode;
   }

   /**
    * \brief test function for the BaseMetadata class
    * \return true on success, false on failure
    **/
   bool testImageContainer()
   {
      ImageMetadata metadata;
      metadata.m_id = 1234;
      metadata.m_offset = 1235;
      metadata.m_mode = 1;
      metadata.m_width= 2;
      metadata.m_height= 3;
      metadata.m_bpp= 4;

      std::string expected("{\"id\":1234,\"offset\":1235,\"elementSize\":1,\"image\":{\"mode\":1,\"width\":2,\"height\":3,\"bpp\":4}}");
      std::string result = metadata.getJsonString();

      if( !expected.compare(result)) {
         return true;
      }

      std::cout << "testImageMetadata failed.\n"
                << "- result:   "<<result<<"\n"
                << "- expected: "<<expected<<std::endl;
      return false;
   }
};
