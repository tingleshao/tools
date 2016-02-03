#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <cstdlib>
#include <iostream>

/**
 * \brief Base class to handle data management for abstract data types
 **/
template <typename T>
class DataBuffer
{
   protected:
      size_t   m_bufferSize     = 0;     //!< Size of the allocated buffer in bytes
      size_t   m_allocatedElements = 0;     //!< Number of elements in the buffer based on T
      T *      m_buffer = NULL;          //!< Pointer to buffer data

   public:
      ~DataBuffer();
      T    operator [](size_t index) const   {return m_buffer[index];};
      T    & operator [](size_t index) {return m_buffer[index];};

    
      size_t  getAllocatedElements();
      virtual bool allocate( size_t elements, bool resizeFlag = false );
      virtual void deallocate();
};

using namespace std;
/**
 * \brief Destructor
 *
 * Makes sure all allocated data gets removed
 **/
template<typename T>
DataBuffer<T>::~DataBuffer() 
{
   if( m_buffer != NULL ) {
      deallocate();
   }
}

template<typename T>
size_t DataBuffer<T>::getAllocatedElements(void)
{
   return m_allocatedElements;
}

/**
 * \brief Allocates the buffer to the given number of elements. 
 *
 * \param [in] size number of elements of the base type to allocate
 * \param [in] resizeFlag flag to indicate if data should be deleted and reallocated
 * \return true on success, false on failure
 **/
template<typename T>
bool DataBuffer<T>::allocate( size_t elements, bool resizeFlag)
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

   size_t bytes = sizeof(T)*elements;

   void * mem = std::realloc(m_buffer, bytes);
   if( mem == NULL ) {
      throw std::bad_alloc();
   }

   m_buffer = static_cast<T*>(mem);

   m_allocatedElements = elements;
   m_bufferSize = sizeof( bytes );

   return true;
}

/**
 * \brief Deallocates and allocated data
 **/
template<typename T>
void DataBuffer<T>::deallocate( void )
{
   if( m_buffer == NULL ) {
      return;
   }

   std::free(m_buffer);
   m_buffer = NULL;
   m_allocatedElements = 0;
   m_bufferSize = 0;
}

// Function to test the databuffer operation
bool testDataBuffer(void);
#endif
