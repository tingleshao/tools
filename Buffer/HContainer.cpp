#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>

#include <fcntl.h>
#include <unistd.h>

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
         m_metadata = reinterpret_cast<HContainerMetadata *>(BaseContainer::m_metadata);
         m_metadata->m_tableSize = tableSize;
         m_metadata->m_usedBytes = metaSize+tableSize;
         m_offsetArray = reinterpret_cast<uint16_t *>(&m_buffer[sizeof(HContainerMetadata)]);
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
      if( m_metadata == NULL ) {
         std::cerr << "HContainer metadata has not been allocated" <<std::endl;
         return false;
      }
      //Make sure we are not full 
      if( m_metadata->m_containerCount >= m_metadata->m_tableSize ) {
         std::cerr << "HContainer reached element count("
                   << m_metadata->m_tableSize
                   << "). Cannot add element"<<std::endl;
         return false;
      }

      //Get the container size
      size_t containerSize = container.getSize();

      //Make sure we have space
      if(( m_metadata->m_size - m_metadata->m_usedBytes ) < containerSize)
      {
         std::cerr << "HContainer does not have sufficent space to add element\n"
                   << "   " << m_metadata->m_size - m_metadata->m_usedBytes
                   << " < " << container.getSize() <<std::endl;
         return false;
      }

      container.m_metadata->m_offset += m_metadata->m_usedBytes;

      //Should be good. Copy data in
      std::memcpy( &m_buffer[m_metadata->m_usedBytes]
                 , &container.m_buffer[0]
                 , containerSize
                 );

      m_offsetArray[m_metadata->m_containerCount] = m_metadata->m_usedBytes;
      m_metadata->m_containerCount++;
      m_metadata->m_usedBytes += containerSize;

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

      //Get update the metadata pointer
      size_t offset = m_offsetArray[index];
      container.m_metadata = reinterpret_cast<BaseContainerMetadata *>(&m_buffer[offset]);

      return true;
   }

   /**
    * \brief HContainer Unit test
    **/
   bool testHContainer() {
      HContainer hc;

      //Create a base container object
      std::vector<BaseContainer> conts(3);
      conts[0].allocate(100);
      conts[1].allocate(1000);
      conts[2].allocate(10000);
      
      uint64_t expectedSize = 11100 + 3*sizeof( BaseContainerMetadata);
      size_t total = 0;
      for( unsigned int i = 0; i < conts.size(); i++ ) {
         total += conts[i].getSize();
      }
      if( total != expectedSize ) {
         std::cout << "Total size incorrect ("<<total<<"!="<<expectedSize<<")"<<std::endl;
         return false;
      }

      hc.allocate(total);
      for( unsigned int i = 0; i < conts.size(); i++ ) {
         hc.add( conts[i]);
      }

      BaseContainer bc1;
      hc.getContainer( bc1, 0 );
      bc1.save("test1.tmp");
      conts[0].save("test2.tmp");

      return true;
   }
};
