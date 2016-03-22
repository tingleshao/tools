#pragma once
#include "DataBuffer.h"

namespace atl
{
   /**
    * \brief Templated Extension of the DataBuffer class that supports elements arbitrary size
    **/
   template <typename T>
   class ExtendedBuffer : public DataBuffer
   {
      private: 
         size_t m_elementSize  = 0;         //!< Size of an element
         size_t m_capacity     = 0;         //!< Number of elements in the allocated buffer
         size_t m_maxIndex     = 0;         //!< Highest index specified         
      protected:
      public:
         ExtendedBuffer(size_t elements=0);

         bool   allocate( size_t elements, bool resizeFlag = false);
         void   deallocate();
         size_t getMaxIndex();
         size_t getCapacity();
         size_t setElements( T * array, size_t elements, size_t startIndex, bool resizeFlag = false);
         bool   getElements( T * array, size_t count, size_t startIndex );
         size_t appendBuffer( ExtendedBuffer<T> buffer, bool resizeFlag = true );

         ExtendedBuffer<T>  getCopy( bool releaseFlag = false );

         /** \brief returns the value at the index **/
         T operator [](size_t index) const   {return ((T*)DataBuffer::m_buffer.get())[index];};
         /** \brief assigned the index to the value **/
         T & operator [](size_t index) { return ((T *)DataBuffer::m_buffer.get())[index];};
   };


   /** 
    * \brief Constructor
    *
    * \param [in] elements number of elements to allocate in the array
    *
    * This function allocated the specified number of elements;
    **/
    template<typename T>
    ExtendedBuffer<T>::ExtendedBuffer( size_t elements )
    {
       m_elementSize = sizeof(T);

       if( elements ) {
          allocate(elements);
       }

    }

    /**
     * \brief allocates data for the array
     *
     * \param [in] elements number of elements to allocate
     * \param [in] resizeFlag indicates if we are allowed to resize our memory
     **/
    template<typename T>
    bool ExtendedBuffer<T>::allocate( size_t elements, bool resizeFlag )
    {
       bool rc = DataBuffer::allocate( m_elementSize * elements, resizeFlag );

       if( rc )  {
          m_capacity = m_bufferSize / m_elementSize;
       }

       return rc;
    }

    /**
     * \brief deallocates the buffers
     **/
    template<typename T>
    void ExtendedBuffer<T>::deallocate()
    {
       m_capacity     = 0;
       m_maxIndex     = 0;
       DataBuffer::deallocate();
    }

    /**
     * \brief Returns the maximum specified index
     **/
    template<typename T>
    size_t ExtendedBuffer<T>::getMaxIndex()
    {
       return m_maxIndex;
    }

    /**
     * \brief Returns the allocated capacity of the array
     **/
    template<typename T>
    size_t ExtendedBuffer<T>::getCapacity()
    {
       return m_capacity;
    }

    /**
     * \brief Sets the array at the given index as specified
     *
     * \param [in] array array of elements to insert into the buffer
     * \param [in] elements number of elements in the array
     * \param [in] offset index to start insertion from
     * \param [in] resizeFlag flag to indicate if we need to resize the buffer 
     *
     * return  number of elements added to the array
     **/
    template<typename T>
    size_t ExtendedBuffer<T>::setElements( T * array, size_t elements, size_t startIndex, bool resizeFlag)
    {
       //If we are UINT_MAX, set startIndex to m_maxIndex
       if( startIndex == UINT_MAX ) {
          startIndex = m_maxIndex;
       }

       //Set data as needed
       size_t bytes = DataBuffer::setData((uint8_t *)array
                                         , elements * m_elementSize
                                         , startIndex * m_elementSize
                                         , resizeFlag 
                                         );

       //Check if we need to allocate more data
       if( bytes > 0 ) 
       {
          m_maxIndex = startIndex+elements; 
          m_capacity = m_bufferSize / m_elementSize;
       }

       return bytes/m_elementSize;
    }

    /**
     * \brief Populates the given array with the specified data
     *
     * \param [in] array pointer to the array to fill with info
     * \param [in] count number of elements to get
     * \param [in] startIndex first index to start copy from
     * \return true on success, false on failure
     **/ 
    template<typename T>
    bool ExtendedBuffer<T>::getElements( T * array, size_t count, size_t startIndex )
    {

       memcpy( array, DataBuffer::m_buffer.get(), count * m_elementSize );
       return true;

    }

    /**
     * \brief Gets a copy elements 
     * \param [in] releaseFlag flag to indicate that the orginal class is done with the data
     *
     * This Function returns a copy of the class. The releaseFlag is used to eliminate references
     * to the pointer within the interal class
     **/ 
    template<typename T>
    ExtendedBuffer<T> ExtendedBuffer<T>::getCopy( bool releaseFlag )
    {
       ExtendedBuffer<T> buffer = *this;

       if( releaseFlag ) {
          DataBuffer::deallocate();
       }

       return buffer;
    }

    /**
     * \brief Appends data from the provided buffer onto the end of the current buffer
     *
     * \param [in] buffer ExtendedBuffer (of type U) to append to the end
     * \return offset of the data into the buffer on sucess, UINT_MAX on failure
     **/
    template<typename T>
    size_t ExtendedBuffer<T>::appendBuffer( ExtendedBuffer<T> buffer, bool resizeFlag )
    {
       //Reserve the offset of the data
       size_t offset = m_maxIndex;
       setElements( (T*)buffer.m_buffer.get(), buffer.getMaxIndex(), offset ); 

       return offset;
    }
}

bool testExtendedBuffer();
