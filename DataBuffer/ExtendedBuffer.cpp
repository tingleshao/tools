#include <iostream>
//#include <new>

#include "ExtendedBuffer.h"

using namespace std;

bool testExtendedBuffer()
{
   bool rc = true;

   size_t elements = 100;

   //Create some buffers
   ExtendedBuffer<uint8_t> uint8Buffer;
   
   //Allocate buffers
   //uint8Buffer.allocate(elements);
   uint8Buffer.assignElements( 0, elements);
   size_t count = uint8Buffer.getAllocatedElements();
   if( count != 0 ) {
      std::cerr << "Allocated elements when not supposed to" <<count<<"!= 0" << endl;
      rc = false;
   }

   size_t offset = uint8Buffer.getElementCount();

   //Force reset from current offset;
   uint8Buffer.assignElements( 0, elements, 0, true );

   for( int i = -2; i < 2; i++ ) {
      if( uint8Buffer[0] != 70 ) 
      {
         std::cerr << "ExtendedBuffer mismatch"<<std::endl;
         rc = false;
      }
   }

   return rc;
}

