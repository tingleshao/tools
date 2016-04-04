#pragma once
#include "TSArray.tcc"

namespace atl
{
   /**
    * !\brief Templated class for representing arrays of arbitrary data
    *
    * This class handle continuous arrays of arbitrary data types
    **/
   template <typename T>
   class TSTable 
   {
      private:
         std::vector<size_t> m_dimensions;       //!< Array of the dimensions of the data
         std::vector<size_t> m_dimSize;        //!< Array of the size for each dimension

      protected:
         
      public:
         bool setDimensions( std::vector<size_t> dims);
         std::vector<size_t> getDimensions();
         bool getItem( T * item, std::vector<size_t> coords);
         bool setItem( T item, std::vector<size_t> coords );
   };

   
   /**
    * \brief Specifies the dimensions of the array and allocates the buffers as needed
    * \return size of the array
    *
    * This function allows external processes to get the size of the array
    **/
   template <typename T>
   std::vector<size_t> TSTable<T>::getDimensions()
   { 
      return m_dimensions;
   }

   /**
    * \brief Specifies the dimensions of the array and allocates the buffers as needed
    *
    * \param [in] dims vector of the dimensions of the object
    * \return true on success, false on failure
    *
    * This function allows external processes to get the size of the array
    **/
   template <typename T>
   bool TSTable<T>::setDimensions( std::vector<size_t>dims)
   { 
      bool rc = true;

      //Make sure we are not reallocating
      if(( m_dimensions.size() != 0 )||(dims.size() == 0 )) {
         std::cerr << "TSTable::setDimensions array already defined."<<std::endl;
         rc = false;
      } 
      else {
         m_dimensions = dims;
         m_dimSize = dims;
         m_dimSize[dims.size()-1] = 0;

         //Loop to calculate the total size
         size_t totalSize = 1;
         for( int i = m_dimensions.size()-1; i >= 0; i-- ) {
            if( i == m_dimensions.size()-1) {
               m_dimSize[i] = totalSize;
            }
            else {
               m_dimSize[i] = totalSize;
            }

            //Calculate total size
            totalSize *= m_dimensions[i];
         }

         TSArray<T>::setSize(totalSize);
         rc = true;
      }
      return rc;
   }

   /**
    * \brief Calculates the offset into based on the provided vector
    *
    * \param [in] coords vector of coordinate values
    **/
   template <typename T>
   size_t TSTable<T>::calculateOffset( std::vector<size_t> coords)
   {
      //Find a total offset into the array
      size_t offset = 0;
      for( int i = 0; i < m_dimSize.size(); i++ ) {
         offset += coords[i]*m_dimSize[i];
      }

      return offset;
   }
   
   /**
    * \brief Gets the item at the specified coordinates
    *
    * \param [in] item pointer to the item at the specified index
    * \param [in] coords vector of coordinates of the item.
    * \return true on success, false on failure
    **/
   template <typename T>
   bool TSTable<T>::getItem( T * item, std::vector<size_t> coords)
   {
      if( coords.size() != m_dimensions.size()) {
         std::cerr << "TSArray getItem: requested coordinates do not match"<<std::endl;
         return false;
      }

      size_t offset = calculateOffset( coords);

      return TSArray<T>::getItem( item, offset );
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
   bool TSTable<T>::setItem( T item, std::vector<size_t> coords)
   {
      return TSArray<T>::setItem( item, calculateOffset(coords));
   }
}

//Test functionality
bool testTSTable();
