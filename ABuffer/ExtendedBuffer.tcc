#ifndef EXTENDEDBUFFER_H
#define EXTENDEDBUFFER_H
//#include <iostream>
#include <cstdlib>
#include <iostream>
#include <climits>
#include <cstring>
#include <vector>
#include <algorithm>
#include "TypeBuffer.tcc"

using namespace std;

namespace atl
{

   /**
    * \brief Extends the DataBuffer class to manage arrays of data.
    *
    * This class is templated so data can be accessed in terms of elements
    * as opposed to being restricted to a single type
    **/
   template <typename T>
   class ExtendedBuffer : public TypeBuffer<T>
   {
      protected:
      public:
         ExtendedBuffer(size_t elements=0);
   
         bool   allocate( size_t elements, bool resizeFlag = false);
         void   deallocate();
         size_t getSize();
         size_t getMaxIndex();
         bool   setMaxIndex( size_t value );
         size_t setElements( std::vector<T> &array, size_t startIndex, bool resizeFlag=true );
         std::vector<T> getElements( size_t count, size_t startIndex );
   
         TypeBuffer<T> getTypeBuffer( bool release = false );
   
   
         /** \brief returns the value at the index **/
         T operator [](size_t index) const   {return TypeBuffer<T>::m_bufferVect->at(index);}; 
         /** \brief assigned the index to the value **/
         T & operator [](size_t index) { 
                                          return TypeBuffer<T>::m_bufferVect->at(index);
                                       }; 
   
   };
   
   /**
    * \brief Sets the number of elements to the specified value
    *
    * \param [in] elements number of elements to allocate on creation (default = 0 );
    **/
   template<typename T>
   ExtendedBuffer<T>::ExtendedBuffer( size_t elements) 
   {
      allocate(elements);
   }
   
   /**
    * \brief Returns the number of elements allocated to the array
    * \return number of elements allocated in the array
    **/
   template<typename T>
   size_t ExtendedBuffer<T>::getSize()
   {
      if( TypeBuffer<T>::m_bufferVect.use_count() == 0 ) {
         return 0;
      }

      return TypeBuffer<T>::m_bufferVect->size();
   }
   
   /**
    * \brief allocates the buffer and sets the elementCount
    *
    * \param [in] elements number of elements to allocate
    * \param [in] resizeFlag boolean to indicate if we resize as needed
    * \return true on success, false on failure
    *
    * This functions  wraps the DataBuffer::allocate call to provide a method
    * to set the element count. This data will automatically be allocated if
    * the underlying buffer is set to NULL
    **/
   template<typename T>
   bool ExtendedBuffer<T>::allocate( size_t elements, bool resizeFlag )
   {
      //Always allocate and reserve the provided number of elements on initialization
      if( TypeBuffer<T>::m_bufferVect.use_count() == 0 ) {
         TypeBuffer<T>::m_bufferVect.reset(new std::vector<T>);
         TypeBuffer<T>::m_bufferVect->reserve(elements);
      }
      else if( resizeFlag ) {
         TypeBuffer<T>::m_bufferVect->resize(elements);
      }
   
      return true;
   }
   
   /**
    * \brief deallocates the buffer and clears the elementCount
    *
    * \return true on success, false on failure
    *
    * This functions  wraps the DataBuffer::deallocate call to provide a method
    * to clear the element count.
    **/
   template<typename T>
   void ExtendedBuffer<T>::deallocate()
   {
      return;
   }
   
   /**
    * \brief This function copies an array of elements to the specified offset
    *
    * \param [in] array pointer to the array of elements to copy
    * \param [in] count number of elements to copy
    * \param [in] startIndex index to start to copy to
    * \param [in] resizeFlag flag to indicate if array should be resized if needed.
    * \return number of elements copied.
    *
    * If the resize flag is set, this function well expand the array to handle all
    * of the input data. Otherwise, if the internal buffer is undersized, all 
    * available space will be filled. If startIndex == UINT_MAX, the array is appended to 
    * the current m_maxIndex index. 
    **/
   template<typename T>
   size_t ExtendedBuffer<T>::setElements( std::vector<T> &array, size_t startIndex, bool resizeFlag ) 
   {
      //If we are not used, allocate
      if( TypeBuffer<T>::m_bufferVect.use_count() == 0 ) {
         allocate(array.size());
      }

      else if( TypeBuffer<T>::m_bufferVect->size() < startIndex + array.size())
      {
         allocate( startIndex + array.size(), true);
//         TypeBuffer<T>::m_bufferVect->resize(  
      }

      std::copy( array.begin()
               , array.end()
               , TypeBuffer<T>::m_bufferVect->begin()+startIndex
               );
   
      return TypeBuffer<T>::m_bufferVect->size();
   }
   
   /**
    * \brief Returns a copy of the elements at the given destination
    *
    * \param [in] dest destination address to write data to
    * \param [in] count number of elements to copy 
    * \param [in] startIndex index into array to start with (default=0)
    * \return number of elements successfully copied
    *
    * This function copies the number of specified elements to the destination
    * address starting at the index specified by the startIndex. If startIndex 
    * is greater that the number of specified elements (elementCount) this only
    * the elements that are defined are copied. 
    *
    * No error checking is performed to ensure that the destination pointer has
    * been properly allocated.
    **/
   template<typename T>
   std::vector<T> ExtendedBuffer<T>::getElements( size_t count, size_t startIndex ) 
   {
      std::vector<T> localVect;
      localVect.reserve(TypeBuffer<T>::m_bufferVect.size());
      localVect.insert( localVect.begin(), TypeBuffer<T>::m_bufferVect.begin(), TypeBuffer<T>::m_bufferVect.end());

      return localVect; 
   }
   
   /**
    * \brief Gets a pointer to the buffer and clears class values (no deallocation)
    * \return TypeBuffer with the data pointer and number fo elements
    **/
   template <typename T>
   TypeBuffer<T> ExtendedBuffer<T>::getTypeBuffer( bool release)
   {
      TypeBuffer<T> tbuffer = *this;

      if( release ) 
      {
         //Clear Extended Buffer variables
         //Clear DataBuffer variables
         TypeBuffer<T>::m_bufferVect.reset();
      }

      return tbuffer;
   }
   
}

//Test function
bool testExtendedBuffer();
#endif
