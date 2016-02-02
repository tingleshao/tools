#ifndef DATABUFFER_H
#define DATABUFFER_H
//#include <iostream>
#include <cstdlib>
#include <iostream>

/**
 * \brief Base class to handle data management for abstract data types
 **/
template <typename T>
class DataBuffer
{
   public:
      size_t   m_bufferSize     = 0;     //!< Size of the allocated buffer in bytes
      size_t   m_bufferElements = 0;     //!< Number of elements in the buffer based on T
      T *      m_buffer = NULL;          //!< Pointer to buffer data

      ~DataBuffer();
      T    operator [](size_t index) const   {return m_buffer[index];};
      T    & operator [](size_t index) {return m_buffer[index];};

      bool allocate( size_t elements, bool force = false );
      void deallocate();
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

/**
 * \brief Allocates the buffer to the given number of elements. 
 *
 * \param [in] size number of elements of the base type to allocate
 * \param [in] force flag to indicate if data should be deleted and reallocated
 * \return true on success, false on failure
 **/
template<typename T>
bool DataBuffer<T>::allocate( size_t elements, bool force)
{
   //Check to make sure we are not already allocated
   if( m_buffer != NULL ) {
      if( !force ) {
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

   m_bufferElements = elements;
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
   m_bufferElements = 0;
   m_bufferSize = 0;
}

// Function to test the databuffer operation
bool testDataBuffer(void);
#endif
