#ifndef EXTENDEDRAWBUFFER_H
#define EXTENDEDRAWBUFFER_H
//#include <iostream>
#include <cstdlib>
#include <iostream>
#include <climits>
#include <cstring>
#include "DataBuffer.h"

using namespace std;

/**
 * !\brief Templated class for representing arrays of arbitrary data
 *
 * This class handle continuous arrays of arbitrary data types
 **/
template <typename T>
class TypeBuffer 
{
   public:
      size_t m_elements = 0;
      T * m_buffer = NULL;                 //!< Pointer to the  buffer in the Base class

      bool allocate( size_t elements, bool resizeFlag = false );
      void deallocate();


      /** \brief returns the value at the index **/
      T operator [](size_t index) const   {return m_buffer[index];}; 
      /** \brief assigned the index to the value **/
      T & operator [](size_t index) {return m_buffer[index];}; 
};

/**
 * \brief Allocates the buffer to the given number of elements. 
 *
 * \param [in] elements number of elements of the base type to allocate
 * \param [in] resizeFlag flag to indicate if data should be deleted and reallocated
 * \return true on success, false on failure
 *
 * This function allocates the number of elements specified is the buffer is NULL. If the
 * resize flag is set, it will reallocate the data and maintain existing info.
 **/
template <typename T>
bool TypeBuffer<T>::allocate( size_t elements, bool resizeFlag)
{
   //Check to make sure we are not already allocated
   if( m_buffer != NULL ) {
      if( !resizeFlag ) {
         return false;
      }
   }

   //Check if reallocating to zero. If so, deallocate
   if( elements == 0 ) {
      deallocate();
      return true;
   }

   //If we need more elements
   if( elements > m_elements ) {
      void * mem = std::realloc(m_buffer, elements  * sizeof(T));
      if( mem == NULL ) {
         throw std::bad_alloc();
      } 
      else {
         m_buffer = static_cast<T *>(mem);
         m_elements = elements;
      }
   }

   return true;
}


/**
 * \brief Deallocates and allocated data
 **/
template <typename T>
void TypeBuffer<T>::deallocate( void )
{
   if( m_buffer == NULL ) {
      return;
   }

   std::free(m_buffer);
   m_buffer = NULL;
   m_elements = 0;
}

//Test functionality
bool testTypeBuffer();
#endif
