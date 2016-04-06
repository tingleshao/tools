#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>

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
    * \brief Adds any container derived from the BaseContainer class
    * \param [in] container object to add
    * \return true on success, false on failure
    **/ 
   bool HContainer::add( BaseContainer & container )
   {
      //Make sure we are not full 
      if( m_metadata->m_containerCount >= m_metadata->m_maxContainers ) {
         std::cerr << "HContainer reached element count("
                   << m_metadata->m_maxContainers
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

      //Should be good. Copy data in
      std::memcpy( &m_buffer[m_metadata->m_usedBytes]
                 , &container.m_buffer[0]
                 , containerSize
                 );

      offsetArray[m_metadata->m_containerCount] = m_metadata->m_usedBytes;
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
//   BaseContainer HContainer::getContainer( uint64_t index )
   {
      //Get a copy of the container
      container = *this;

      //Get update the metadata pointer
      container.m_metadata = m_metadata + offsetArray[index];

      return true;
   }

   /**
    * \brief HContainer Unit test
    **/
   bool testHContainer() {
      HContainer hc;

      //Create a base container object
      BaseContainer cont1;
      cont1.allocate(100);

      hc.add(cont1);
      return false;
   }
};
