#pragma once
#include <memory>
#include <climits>
#include <stddef.h>
/** 
 * \file 
 * \copyright 2016 Aqueti, Incorporated
 * \license The MIT License (MIT)
 **/

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
   class BaseBuffer
   {
      private:
         
      public: 
         size_t m_bufferSize      = 0;                      //!< Number of elements in the buffer
         std::shared_ptr<uint8_t> m_buffer;                 //!< Actual data buffer
   
         bool allocate( size_t bytes, bool resizeFlag = false);
         void deallocate();
         size_t getSize();
   
         /** \brief returns the value at the index **/
         uint8_t    operator [](size_t index) const   {return m_buffer.get()[index];}; 
         /** \brief assigned the index to the value **/
         uint8_t    & operator [](size_t index) {return m_buffer.get()[index];}; 
   
   };


   //Test functions
   bool testBaseBuffer();
};
