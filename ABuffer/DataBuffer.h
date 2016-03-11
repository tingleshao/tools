#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <memory>
#include <climits>
#include <stddef.h>

#include "BaseBuffer.h"

namespace atl
{
   /**
    * \brief Base class to handle data management for abstract data types
    **/
   class DataBuffer : public BaseBuffer
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
         bool    getData( std::shared_ptr<uint8_t> &buffer, size_t &bytes );
         
         BaseBuffer getData();
   
         bool allocate( size_t bytes, bool resizeFlag = false );
   };

   //Test functions
   bool testDataBuffer();
}

#endif
