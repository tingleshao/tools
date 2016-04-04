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
   class TSMatrix : public TSArray<T>
   {
      private:
         std::vector<size_t> m_dimensions;       //!< Array of the dimensions of the data
         std::vector<size_t> m_dimScalar;        //!< Array of the scalars for each dimension

         size_t calculateOffset( std::vector<size_t> coords);
         
      public:
         bool setDimensions( std::vector<size_t> dims);
         std::vector<size_t> getDimensions();
         bool getItem( T &item, std::vector<size_t> coords);
         bool setItem( T &item, std::vector<size_t> coords );
   };

   
   /**
    * \brief Specifies the dimensions of the array and allocates the buffers as needed
    * \return size of the array
    *
    * This function allows external processes to get the size of the array
    **/
   template <typename T>
   std::vector<size_t> TSMatrix<T>::getDimensions()
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
   bool TSMatrix<T>::setDimensions( std::vector<size_t>dims)
   { 
      bool rc = true;

      //Make sure we are not reallocating
      if(( m_dimensions.size() != 0 )||(dims.size() == 0 )) {
         std::cerr << "TSMatrix::setDimensions array already defined."<<std::endl;
         rc = false;
      } 
      else {
         m_dimensions = dims;
         m_dimScalar = dims;
         m_dimScalar[dims.size()-1] = 0;

         //Loop to calculate the total size
         size_t totalSize = 1;
         for( size_t i = m_dimensions.size()-1; i > 0; i-- ) {
            if( i == m_dimensions.size()-1) {
               m_dimScalar[i] = totalSize;
            }
            else {
               m_dimScalar[i] = totalSize;
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
   size_t TSMatrix<T>::calculateOffset( std::vector<size_t> coords)
   {
      //Find a total offset into the array
      size_t offset = 0;
      for( size_t i = 0; i < m_dimScalar.size(); i++ ) {
         offset += coords[i]*m_dimScalar[i];
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
   bool TSMatrix<T>::getItem( T &item, std::vector<size_t> coords)
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
   bool TSMatrix<T>::setItem( T &item, std::vector<size_t> coords)
   {
      return TSArray<T>::setItem( item, calculateOffset(coords));
   }
}

//Test functionality
bool testTSMatrix();
