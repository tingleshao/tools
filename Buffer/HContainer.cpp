#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>

#include <fcntl.h>
#include <unistd.h>

#include <ImageContainer.h>
#include "HContainer.h"

namespace atl
{
   /**
    * \brief Sets the default blocks size
    **/
   HContainer::HContainer()
   {
      m_blockSize = BLK_SIZE_4MB;
   }

   /**
    * \brief allocates the buffer and re-interprets pointer
    **/
   size_t HContainer::allocate( size_t bytes, uint16_t tableSize, uint64_t blockSize )
   {
      if( blockSize != BLK_SIZE_DEFAULT ) {
         m_blockSize = blockSize;
      }

      //Call the base allocator
      uint64_t metaSize = sizeof( HContainerMetadata);

      size_t result = BaseContainer::allocate( bytes, m_blockSize, metaSize + tableSize );
      if( result != 0 ) {
         HContainerMetadata * meta = reinterpret_cast<HContainerMetadata *>(BaseContainer::m_metadata);
         meta->m_tableSize = tableSize;
         meta->m_usedBytes = metaSize+tableSize;
         meta->m_metaSize  = metaSize;
         m_offsetArray = reinterpret_cast<uint16_t *>(&m_buffer[meta->m_metaSize]);
      }

      return result;
   }

   /**
    * \brief Adds any container derived from the BaseContainer class
    * \param [in] container object to add
    * \return true on success, false on failure
    **/ 
   bool HContainer::add( BaseContainer & container )
   {
      //Map to HContainer metadata
      HContainerMetadata * meta = reinterpret_cast<HContainerMetadata *>(BaseContainer::m_metadata);
      if( meta == NULL ) {
         std::cerr << "HContainer metadata has not been allocated" <<std::endl;
         return false;
      }

      //Make sure we are not full 
      if( meta->m_containerCount >= meta->m_tableSize ) {
         std::cerr << "HContainer reached element count("
                   << meta->m_tableSize
                   << "). Cannot add element"<<std::endl;
         return false;
      }

      //Get the container size
      size_t containerSize = container.m_metadata->m_size;
      std::cout << "   container size:"<< containerSize << std::endl;
      std::cout << "   meta_usedBytes:"<< meta->m_usedBytes << std::endl;

      //Make sure we have space
      if(( meta->m_size - meta->m_usedBytes ) < containerSize)
      {
         std::cerr << "HContainer does not have sufficent space to add element\n"
                   << "   " << meta->m_size - meta->m_usedBytes
                   << " < " << container.getSize() <<std::endl;
         return false;
      }

      //Copy data to the end
      std::memcpy( &m_buffer[meta->m_usedBytes]
                 , &container.m_buffer[0]
                 , containerSize
                 );

      BaseContainerMetadata * contMeta = reinterpret_cast<BaseContainerMetadata *>(&m_buffer[meta->m_usedBytes]);
      contMeta->m_offset += meta->m_usedBytes;

      //Set the lookup table to point to the new data and update variables
      m_offsetArray[meta->m_containerCount] = meta->m_usedBytes;
      meta->m_usedBytes = meta->m_usedBytes + containerSize;
      meta->m_containerCount++;

      return true;
   }

   /**
    * \brief Returns the base container object at the specified index
    * \param [in] index index of the container to retrieve
    * \return true on success, false on failure
    **/
   bool HContainer::getContainer( BaseContainer &container, uint64_t index )
   {
      //Get a copy of the container
      container = *this;

      //Get a pointer to the memory
      size_t offset = m_offsetArray[index];
      BaseContainerMetadata * metadata = reinterpret_cast<BaseContainerMetadata *>(&m_buffer[offset]);
      std::cout << "++++PixelSize:"<<((ImageMetadata *)metadata)->m_pixelDataSize;
      createStaticMetadata( metadata->m_metaSize );
      memcpy( container.m_metadata, metadata, metadata->m_metaSize );
//      container.m_metadata->m_offset += offset;

      return true;
   }

   /**
    * \brief HContainer Unit test
    **/
   bool testHContainer() {
      HContainer hc;

      std::vector<uint16_t>dims={256,512,256};

      std::vector<ImageContainer> conts;
      size_t pixSize = 0;
      for( unsigned int i = 0; i < dims.size(); i++ )
      {
         ImageContainer c;
         conts.push_back(c);
         conts[i] = genTestImage(dims[i], dims[i]);
         pixSize += dims[i]*dims[i];

      }
      
      
      uint64_t expectedSize = pixSize + 3*sizeof( ImageMetadata);
      size_t total = 0;
      for( unsigned int i = 0; i < conts.size(); i++ ) {
         total += conts[i].getSize();
      }
      if( total != expectedSize ) {
         std::cout << "Total size incorrect ("<<total<<"!="<<expectedSize<<")"<<std::endl;
         return false;
      }

      conts[0].savePNM("C0_ref.pnm", conts[0].getJsonMetadata());
      bool rc = true;
      hc.allocate(total);
      for( unsigned int i = 0; i < conts.size(); i++ ) {
         std::cout << "   Adding size:"<< conts[i].m_metadata->m_size << std::endl;
         rc = hc.add( conts[i]);
         if( !rc ) {
            std::cerr <<"Unable to add container with Id: "<<i<<std::endl;
            return false;
         }
      }
      conts[0].savePNM("C0b_ref.pnm", conts[0].getJsonMetadata());

      std::string jsonString = conts[0].getJsonMetadata();
      if( !conts[0].savePNM("test1b.pnm", jsonString ))
      {
         std::cout << "Failed to save container PNM"<<std::endl;
      }
      ImageContainer bc1;
      hc.getContainer( bc1, 0 );

      std::string json1 = conts[0].getJsonMetadata();
      std::string json2 = bc1.getJsonMetadata();

      //Make sure not equal (offsets changed)
      if( json1.compare(json2)) {
         std::cout << "JSON Strings do not match\n" 
                   << "json1: "<<json1.c_str() << "\n"
                   << "json2: "<<json2.c_str() << std::endl;
         return false;
     }

      ImageContainer recon = genTestImage();
      if( !recon.savePNM("ref.pnm", recon.getJsonMetadata())) 
      {
         std::cout << "Failed to save reference PNM"<<std::endl;
      }


      if( !conts[0].savePNM("test1.pnm", json1.c_str())) {
         std::cout << "Failed to save container PNM"<<std::endl;
      }
      if( !bc1.savePNM("test2.pnm", json2.c_str())) {
         std::cout << "Failed to save copy PNM"<<std::endl;
      }

      conts[0].save("c0.tmp");
      bc1.save("bc1.tmp");

      return true;
   }
};
