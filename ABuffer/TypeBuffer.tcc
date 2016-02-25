#ifndef EXTENDEDRAWBUFFER_H
#define EXTENDEDRAWBUFFER_H
//#include <iostream>
#include <cstdlib>
#include <iostream>
#include <climits>
#include <cstring>
#include <vector>
#include <memory>

using namespace std;

namespace atl
{
   /**
    * !\brief Templated class for representing arrays of arbitrary data
    *
    * This class handle continuous arrays of arbitrary data types
    **/
   template <typename T>
   class TypeBuffer 
   {
      protected:
         std::shared_ptr<std::vector<T>> m_bufferVect; //!< Vector to the buffer
   
         /** \brief returns the value at the index **/
         T operator [](size_t index) const   {return m_bufferVect[index];}; 
         /** \brief assigned the index to the value **/
         T & operator [](size_t index) {return m_bufferVect[index];}; 

      public:
         /** \brief returns the pointer to first element of the buffer **/
         T* getPointer()      { return &m_bufferVect->at(0); };

         /** \brief returns the size of the array **/
         size_t getSize()     {return m_bufferVect->size();};
         /** \brief returns the capacity of the array **/
         size_t getCapacity() {return m_bufferVect->capacity();};

   };
}
   
//Test functionality
bool testTypeBuffer();
#endif
