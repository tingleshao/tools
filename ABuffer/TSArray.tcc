#pragma once
#include <iostream>
#include <climits>
#include <vector>
#include <mutex>

using namespace std;

namespace atl
{
   /**
    * !\brief Templated class for representing arrays of arbitrary data
    *
    * This class handle continuous arrays of arbitrary data types
    **/
   template <typename T>
   class TSArray 
   {
      private:
         std::mutex     m_mutex;                 //!< mutex to ensure thread-safe operation
         std::vector<T> m_array;                 //!< Array of objects
         
      public:
         //function
         bool   setSize( size_t size );
         size_t getSize();
         bool   setItem( T item, size_t index, double waitTime = 0 );
         bool   getItem( T* itemPtr, size_t index, double waitTime = 0);
   };


   /**
    * \brief Returns the number of allocated elements in the array
    * \return size of the array
    *
    * This function allows external processes to get the size of the array
    **/
   template <typename T>
   bool TSArray<T>::setSize( size_t size )
   {
      m_mutex.lock();
      m_array.resize(size);
      m_mutex.unlock();
      return true;
   }
   
   /**
    * \brief Returns the number of allocated elements in the array
    * \return size of the array
    *
    * This function allows external processes to get the size of the array
    **/
   template <typename T>
   size_t TSArray<T>::getSize()
   {
      return m_array.size();
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
   bool TSArray<T>::setItem( T item, size_t index, double waitTime )
   {
      if( m_array.size() > index ) 
      {
         m_mutex.lock();
         m_array[index] = item;
         m_mutex.unlock();
         return true;
      }

      return false;
   }

   /**
    * \brief gets the item at the specified index
    *
    * \param [in] itemPtr pointer to an element to set
    * \param [in] index array index to get data element from
    * \param [in] waiTime amount of time to wait (default=0);
    **/ 
   template <typename T>
   bool TSArray<T>::getItem( T * itemPtr, size_t index, double waitTime)
   {
      //Check array size to see if we have the specified value
      if( index > m_array.size()) {
         cerr << "TSArray index size exceeds array size"<<endl;
         return false;
      }

      //All is good. Extract value
      m_mutex.lock();
      *itemPtr = m_array[index];
      m_mutex.unlock();

      return true;
   }
   
   //Test functionality
}

bool testTSArray();
