#include <iostream>
#include "ExtendedBuffer.h"

using namespace std;

/**
 * \brief Unit test fuction for the ExtendedBuffer class
 **/
bool testExtendedBuffer()
{
   size_t elements = 100;

   //Create some buffers
   ExtendedBuffer<uint8_t> uint8Buffer;
   
   //Allocate buffers - test default resize flag operation
   uint8Buffer.assignElements( 0, elements);
   size_t count = uint8Buffer.getElementCount();
   if( count != 0 ) {
      std::cerr << "Allocated elements when not supposed to" <<count<<"!= 0" << endl;
      return false;
   }

   //Preallocate a buffer without assigning values
   uint8Buffer.allocate( elements );
   count = uint8Buffer.getElementCount();
   if( count != elements ) {
      std::cerr << "Allocated elements incorrect: " <<count<<"!="<<elements<< endl;
      return false;
   }

   //Assign elements with a force resize. New elementSize shoudl reflect elements
   uint8Buffer.assignElements( 0, elements/2, 0, false);
   count = uint8Buffer.getElementCount();
   size_t ecount = uint8Buffer.getMaxIndex();
   if(( count != elements )||(ecount != elements/2)) {
      std::cerr << "Allocated elements incorrect: " <<count<<"!="<<elements
                << "||"<<ecount<<"!="<<elements/2<< endl;
      return false;
   }

   //Assign another set of elements to array. Should top out at count elements
   uint8Buffer.assignElements( 1, elements, ecount, false);
   count = uint8Buffer.getElementCount();
   ecount = uint8Buffer.getElementCount();
   if(( count != elements )||(ecount != elements)) {
      std::cerr << "Allocated elements2 incorrect: " <<count<<"!="<<elements
                << "||"<<ecount<<"!="<<elements<< endl;
      return false;
   }

   //Assign one more set, this time  force another write
   uint8Buffer.assignElements( 2, elements, ecount, true );
   count = uint8Buffer.getElementCount();
   ecount = uint8Buffer.getElementCount();
   if(( count != elements*2 )||(ecount != elements*2)) {
      std::cerr << "Allocated elements2 incorrect: " <<count<<"!="<<elements*2
                << "||"<<ecount<<"!="<<elements*2<< endl;
      return false;
   }

   //Check some variables to see if we're getting good data
   if(( uint8Buffer[0] != 0 ) ||
      ( uint8Buffer[elements/2] != 1 ) ||
      ( uint8Buffer[elements] != 2 ) ||
      ( uint8Buffer[elements*2-1] != 2 )) {
      std::cerr << "ExtendedBuffer failure:\n"
                << "\t" << (int)uint8Buffer[0] <<"\n"
                << "\t" << (int)uint8Buffer[elements/2] <<"\n"
                << "\t" << (int)uint8Buffer[elements] <<"\n"
                << "\t" << (int)uint8Buffer[elements*2-1] <<"\n";

      return false;
   }

   //Try a valid elementCount
   if( uint8Buffer.setMaxIndex ( elements*10)) {
      std::cerr << "False success setting elementCount" << std::endl;
      return false;
   }

   //Try a valid elementCount
   if( ! uint8Buffer.setMaxIndex( elements/2)) {
      std::cerr << "Failed decrementing elementCount" << std::endl;
      return false;
   }

   return true;
}

