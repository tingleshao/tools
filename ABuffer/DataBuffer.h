#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <climits>
#include <stddef.h>

namespace atl
{
   /**
    * \brief Low level data structure to associate pointer with a data size
    *
    * This class is a very simple interface to represent a continuous array of data.
    * The size value is the number of elements in the buffer and the buffer is the data
    * itself. There is no inherent method for data management allocated data is must be
    * freed with an external call to the deallocate function
    **/
   class RawDataBuffer
   {
      public: 
         size_t m_bufferSize      = 0;                    //!< Number of elements in the buffer
         uint8_t * m_buffer = NULL;                 //!< Actual data buffer
   
         bool allocate( size_t bytes, bool resizeFlag = false);
         void deallocate();
   
         /** \brief returns the value at the index **/
         uint8_t    operator [](size_t index) const   {return m_buffer[index];}; 
         /** \brief assigned the index to the value **/
         uint8_t    & operator [](size_t index) {return m_buffer[index];}; 
   
   };
   
   
   /**
    * \brief Base class to handle data management for abstract data types
    **/
   class DataBuffer : public RawDataBuffer
   {
      protected:
         uint8_t   m_defaultValue = false;        //!< Default value
         bool      m_useDefaultValueFlag = false; //!< Flag to indicate if we should use default value
   
      public:
   
         DataBuffer(size_t elements=0);
         ~DataBuffer();
         
         void    useDefaultValue( bool flag);
         void    setDefaultValue(uint8_t value );
         size_t  setData( void * array, size_t count, size_t startIndex = UINT_MAX, bool resizeFlag = false );
         size_t  getData( void * dest, size_t count, size_t startIndex=0 );
         size_t  getSize();
         RawDataBuffer getData( size_t offset=0);
   
         bool allocate( size_t bytes, bool resizeFlag = false );
   };
   //Test functions
   bool testDataBuffer();
}
#endif
