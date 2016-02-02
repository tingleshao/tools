#ifndef DATABUFFER_H
#define DATABUFFER_H
//#include <iostream>
#include <iostream>
#include <new>

/**
 * \brief Base class to handle data management for abstract data types
 **/
template <typename T>
class DataBuffer
{
   public:
      size_t   m_bufferSize     = 0;            //!< Size of the allocated buffer in bytes
      size_t   m_bufferElements = 0;            //!< Number of elements in the buffer based on T
      T *      m_buffer = NULL;                 //!< Pointer to buffer data

      ~DataBuffer();
      T    operator [](size_t index) const   {return m_buffer[index];};
      T    & operator [](size_t index) {return m_buffer[index];};

      bool allocate( size_t elements, bool reallocate = false );
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
 * \brief Allocates the buffer to the give size
 *
 * \param [in] size number of elements of the base type to allocate
 * \param [in] force flag to indicate if data should be deleted and reallocated
 * \return true on success, false on failure
 **/
template<typename T>
bool DataBuffer<T>::allocate( size_t size, bool force )
{
   //Check to make sure we are not already allocated
   if( m_buffer != NULL ) {
      if( force ) {
         deallocate();
      }
      else {
         return false;
      }
   }

   //Allocate new data
   m_buffer = new(std::nothrow) T[size];
   if( m_buffer == NULL ) {
      return false;
   }
   m_bufferElements = size;
   m_bufferSize = sizeof( m_buffer);

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

   delete[] m_buffer;
   m_buffer = NULL;
   m_bufferElements = 0;
   m_bufferSize = 0;
}

bool testDataBuffer(void);
#endif
