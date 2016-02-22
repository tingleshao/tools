#include <iostream>
#include "ExtendedBuffer.tcc"

using namespace std;
namespace atl
{
   
   /**
    * \brief Unit test fuction for the ExtendedBuffer class
    **/
   bool testExtendedBuffer()
   {
      size_t elements = 100;
   
      //Create some buffers
      ExtendedBuffer<uint16_t> uint16Buffer;
      
      //Allocate buffers - test default resize flag operation
      uint16Buffer.assignElements( 10, 10);
      size_t count = uint16Buffer.getMaxIndex();
      if( count != 10 ) {
         std::cerr << "Allocated elements when not supposed to" <<count<<"!= 0" << endl;
         return false;
      }
   
      //Preallocate a buffer without assigning values
      uint16Buffer.allocate( elements, true );
      count = uint16Buffer.getElementCount();
      if( count != elements ) {
         std::cerr << "Allocated elements incorrect: " <<count<<"!="<<elements<< endl;
         return false;
      }
   
      //Assign elements with a force resize. New elementSize shoudl reflect elements
      uint16Buffer.assignElements( 0, elements/2, 0, false);
      count = uint16Buffer.getElementCount();
      size_t ecount = uint16Buffer.getMaxIndex();
      if(( count != elements )||(ecount != elements/2)) {
         std::cerr << "Allocated elements incorrect: " <<count<<"!="<<elements
                   << "||"<<ecount<<"!="<<elements/2<< endl;
         return false;
      }
   
      //Assign another set of elements to array. Should top out at count elements
      uint16Buffer.assignElements( 1, elements, ecount, false);
      count = uint16Buffer.getMaxIndex();
      ecount = uint16Buffer.getMaxIndex();
      if(( count != elements )||(ecount != elements)) {
         std::cerr << "Allocated elements2 incorrect: " <<count<<"!="<<elements
                   << "||"<<ecount<<"!="<<elements<< endl;
         return false;
      }
   
      //Assign one more set, this time  force another write
      uint16Buffer.assignElements( 2, elements, ecount, true );
      count = uint16Buffer.getElementCount();
      ecount = uint16Buffer.getMaxIndex();
      if(( count != elements*2 )||(ecount != elements*2)) {
         std::cerr << "Allocated elements2 incorrect: " <<count<<"!="<<elements*2
                   << "||"<<ecount<<"!="<<elements*2<< endl;
         return false;
      }
   
      //Check some variables to see if we're getting good data
      if(( uint16Buffer[0] != 0 ) ||
         ( uint16Buffer[elements/2] != 1 ) ||
         ( uint16Buffer[elements] != 2 ) ||
         ( uint16Buffer[elements*2-1] != 2 )) {
         std::cerr << "ExtendedBuffer failure:\n"
                   << "\t" << (int)uint16Buffer[0] <<"\n"
                   << "\t" << (int)uint16Buffer[elements/2] <<"\n"
                   << "\t" << (int)uint16Buffer[elements] <<"\n"
                   << "\t" << (int)uint16Buffer[elements*2-1] <<"\n";
   
         return false;
      }
   
      //Try a valid elementCount
      if( uint16Buffer.setMaxIndex ( elements*10)) {
         std::cerr << "False success setting elementCount" << std::endl;
         return false;
      }
   
      //Try a valid elementCount
      if( ! uint16Buffer.setMaxIndex( elements/2)) {
         std::cerr << "Failed decrementing elementCount" << std::endl;
         return false;
      }
   
      //Create a new one to add an array
      ExtendedBuffer<uint8_t> uint8Buffer2;
   //   uint8_t buffer[elements];
      uint8_t buffer[elements];
      for( int i = 0; i <elements; i++ ) 
      {
         buffer[i] = i;
      }
   
      //Try to add elements with default values
      size_t result = uint8Buffer2.setElements(buffer, elements);
      if( result != 0 ) {
         std::cerr<<"Wrote "<<result<<" elements when expecting 0"<<std::endl;
         return false;
      }
   
   
      //Try to add elements with default values
      result = uint8Buffer2.setElements(buffer, elements, 0, true);
      if( result != elements ) {
         std::cerr<<"Wrote "<<result<<" elements when expecting "<<elements<<std::endl;
         return false;
      }
   
      size_t mi = uint16Buffer.getMaxIndex();
      for( int i = 0; i < mi; i ++ ) {
         uint16Buffer[i] = i;
      }
   
      TypeBuffer<uint16_t> tb = uint16Buffer.getTypeBuffer();
      if(( tb.m_buffer == NULL )||(tb.m_elements == 0 )) {
         std::cerr << "Failed to get elements in typebuffer" << std::endl;
         return false;
      }
   
      bool rc = true;
      for( uint8_t i = 0; i < tb.m_elements ; i++ ) {
         if( tb.m_buffer[i] != i ) {
            std::cout << "Element mismatch!: "<<(int)i<<"~="<<(int)tb.m_buffer[i]<<endl;
            rc = false;
         }
   
      }
   
      tb.deallocate();
   
      return rc;
   }
}
