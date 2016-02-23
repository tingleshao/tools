#ifndef EXTENDEDRAWBUFFER_H
#define EXTENDEDRAWBUFFER_H
//#include <iostream>
#include <cstdlib>
#include <iostream>
#include <climits>
#include <cstring>
#include "DataBuffer.h"

using namespace std;

namespace atl
{
   /**
    * !\brief Templated class for representing arrays of arbitrary data
    *
    * This class handle continuous arrays of arbitrary data types
    **/
   template <typename T>
   class TypeBuffer 
   {
      protected:
         size_t m_allocatedElements = 0;              //!< Number of elements in the buffer
         size_t m_maxIndex          = 0;              //!< Highest set index
         std::shared_ptr<std::vector<T>> m_bufferVect; //!< Vector to the buffer
//         const std::shared_ptr<T> m_buffer;         //!< Pointer to the  buffer in the Base class
   
         /** \brief returns the value at the index **/
         T operator [](size_t index) const   {return m_bufferVect[index];}; 
         /** \brief assigned the index to the value **/
         T & operator [](size_t index) {return m_bufferVect[index];}; 
   };
}
   
//Test functionality
bool testTypeBuffer();
#endif
