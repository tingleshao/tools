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
      public:
         size_t m_elements = 0;               //!< Number of elements in the buffer
         std::shared_ptr<T> m_buffer;       //!< Pointer to the  buffer in the Base class
   
         /** \brief returns the value at the index **/
         T operator [](size_t index) const   {return m_buffer.get()[index];}; 
         /** \brief assigned the index to the value **/
         T & operator [](size_t index) {return m_buffer.get()[index];}; 
   };
}
   
//Test functionality
bool testTypeBuffer();
#endif
