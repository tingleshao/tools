#include <DataBuffer.h>

namespace atl
{
   /**
    * \brief class that 
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
         bool   setElements( std::vector<T> &array, size_t startIndex, bool resizeFlag=true );
         std::vector<T> getElements( size_t count, size_t startIndex );

         TypeBuffer<T>  copyBuffer( bool release = false );

         /** \brief returns the value at the index **/
         T operator [](size_t index) const   {return TypeBuffer<T>::m_bufferVect->at(index);};
         /** \brief assigned the index to the value **/
         T & operator [](size_t index) { 
                                          return TypeBuffer<T>::m_bufferVect->at(index);
                                       };  
   }


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

       DataBuffer::allocate( m_dataSize * elements )
       m_maxIndex = 0;
       m_capacity = DataBuffer::getSize() / m_elementSize;
    }

    /**
     * \brief deallocates the buffers
     **/
    template<typename T>
    ExtendedBuffer<T>::deallocate()
    {
       m_capacity     = 0;
       m_maxIndex     = 0;
       DataBuffer::deallocate();
    }

    /**
     * \brief Returns the maximum specified index
     **/
    template<typename T>
    ExtendedBuffer<T>::getMaxIndex()
    {
       return m_maxIndex;
    }

    /**
     * \brief Returns the allocated capacity of the array
     **/
    template<typename T>
    ExtendedBuffer<T>::getCapacity()
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
    size_t ExtendedBuffer<T>::setElements( T* array, size_t elements, size_t startIndex, bool resizeFlag)
    {
       //If we are UINT_MAX, set startIndex to m_maxIndex
       if( startIndex == UINT_MAX ) {
          startIndex = m_maxIndex;
       }

       //Set data as needed
       size_t bytes = DataBuffer::setData(static_cast<uint8_t *>(array)
                                         , elements * m_elementSize
                                         , startIndex * m_elementSize
                                         , resizeFlag 
                                         );

       //Check if we need to allocate more data
       if( bytes > 0 ) 
       {
          m_maxIndex = startIndex+elements; 
          m_capacity = DataBuffer::getSize() / m_elementSize;
       }

       return bytes;
    }

    /**
     * \brief Gets a copy elements 
     **/ 
    template<typename T>
    bool ExtendedBuffer<T>::getElements( T &array, size_t count, size_t startIndex )
    {
       memcpy( array, DataBuffer::m_buffer.get(), count * m_elementSize );
       return true;

    }

    /**
     * \brief Gets a copy elements 
     **/ 
    template<typename T>
    ExtendedBuffer<T> ExtendedBuffer<T>::copyBuffer( bool release = false )
    {
       ExtendedBuffer<T> buffer = this;

       if( release ) {
          DataBuffer::deallocate();
       }

       return buffer;
    }
}
