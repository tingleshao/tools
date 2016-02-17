#pragma once
#include "TSArray.cc"

namespace atl
{
   /**
    * !\brief Templated class for representing arrays of arbitrary data
    *
    * This class handle continuous arrays of arbitrary data types
    **/
   template <typename T>
   class TSMatrix : public TSArray<T>
   {
      private:
         std::vector<size_t> m_dimensions;
         
      public:
         bool setDimensions( std::vector<size_t> dims);
         bool getItem( T * item, std::vector<size_t> coords);
   };

   
   /**
    * \brief Specifies the dimensions of the array and allocates the buffers as needed
    * \return size of the array
    *
    * This function allows external processes to get the size of the array
    **/
   template <typename T>
   bool TSMatrix<T>::setDimensions( std::vector<size_t>dims)
   { 
      bool rc = true;
      m_mutex.lock();

      //Make sure we are not reallocating
      if( dims.size() != 0 ) {
         std::cerr << "TSMatrix::setDimensions array already defined."<<std::endl;
         rc = false;
      } 
      else {
         m_dimensions.assign(dims);
         size_t totalSize = 0;
         for( size_t i = 0; i < m_dimensions.size() {
            totalSize += m_dimensions[i];
         }
         m_array.resize(totalSize);
         rc = true;
      }

      m_mutex.unlock();
      return rc;
   }
   
   /**
    * \brief Gets the item at the specified coordinates
    *
    * \param [in] item pointer to the item at the specified index
    * \param [in] coords vector of coordinates of the item.
    * \return true on success, false on failure
    **/
   template <typename T>
   bool TSArray<T>::getItem( T * item, std::vector<size_t> coords)
   {
      if( coords.size() != m_dimensions.size()) {
         std::cerr << "TSArray getItem: requested coordinates do not match"<<std::endl;
         return false;
      }

      size_t offset = 0;
      size_t sum = 0;
      for( size_t i = 0; i < coords.size(); i++ ) {
         sum += m_dimensions[i];
         offset += 


      m_mutex.lock();

      *T = m_a

      m_mutex.unlock();
   }
   
   /**
    * \brief Sets the array at the specified index to the given item
    *
    * \param [in] item value to set
    * \param [in] index index to set value at
    * \param [in] waitTime amount of time to wait (before giving up)a
    * \return true on success, false on failure
    **/
   template <typename T>
   bool TSArray<T>::setItem( T item, size_t index, double waitTime );
   {
      if( m_array.size() > index ) 
      {
         m_mutex.lock();
         m_array[index] = T;
         m_mutex.lock();
         return true;
      }

      return false;
   }

   /**
    * \brief gets the item at the specified index
    *
    * \param [in] value pointer to an element to set
    * \param [in] index array index to get data element from
    * \param [in] waiTime amount of time to wait (default=0);
    **/ 
   template <typename T>
   bool TSArray<T>::getItem( T * value, size_t index, double waitTime = 0)
   {
      //Check array size to see if we have the specified value
      if( index > m_array.size()) {
         std::cerr << "TSArray index size exceeds array size"<<std::endl;
         return false;
      }

      //All is good. Extract value
      m_mutex.lock( m_mutex);
      *value = m_array[index];
      m_mutex.unlock( m_mutex);

      return true;
   }
   
   //Test functionality
   bool testTSArray();
}
