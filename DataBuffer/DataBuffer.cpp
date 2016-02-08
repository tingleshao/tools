#include <iostream>
//#include <new>

#include "DataBuffer.h"

bool testDataBuffer()
{
   bool rc = true;

   int defaultValue = 10;
   int elements = 100;

   //Create some buffers
   DataBuffer<uint8_t> uint8Buffer;
   DataBuffer<int16_t>int16Buffer;
   DataBuffer<int32_t>int32Buffer;
   DataBuffer<int64_t>int64Buffer;
   DataBuffer<double>DBuffer;
   DataBuffer<char>CBuffer;
   
   uint8Buffer.setDefaultValue(defaultValue );

   //Allocate buffers
   uint8Buffer.allocate(elements);
   int16Buffer.allocate(elements);
   int32Buffer.allocate(elements);
   int64Buffer.allocate(elements);
   DBuffer.allocate(elements);
   CBuffer.allocate(elements);

   if( uint8Buffer[0] != defaultValue ) {
      std::cerr << "Default value not working: "<<(int)uint8Buffer[0]<<"!="<< defaultValue <<endl;
      return false;
   }


   size_t count = uint8Buffer.getElementCount();
   if( count != elements ) {
      std::cerr << "Number of allocated elements does not match:\n\t" 
                <<count<<"!="<<elements << endl;
      rc = false;
   }

   for( int i = -2; i < 2; i++ ) {
      uint8Buffer[i+2] = (uint8_t)i;
      int16Buffer[i+2] = i;
      int32Buffer[i+2] = i;
      int64Buffer[i+2] = i;
      DBuffer[i+2] = i-.5;
      CBuffer[i+2] = i+70;
   }

   for( int i = -2; i < 2; i++ ) {
      if(( uint8Buffer[i+2] != (uint8_t)i ) ||
         ( int16Buffer[i+2] != i ) ||
         ( int32Buffer[i+2] != i ) ||
         ( int64Buffer[i+2] != i ) ||
         ( DBuffer[i+2]  != i-.5 ) ||
         ( CBuffer[i+2]  != i+70 )) {
         std::cerr << "DataBuffer mismatch"<<std::endl;
         rc = false;
      }
   }

   //Create a new one to add an array
   DataBuffer<uint8_t> uint8Buffer2;
   uint8_t * buffer = new uint8_t[elements];
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



   //Deallocate buffers
   uint8Buffer.deallocate();
   int16Buffer.deallocate();
   int32Buffer.deallocate();
   int64Buffer.deallocate();
   DBuffer.deallocate();

   delete buffer;
   return rc;
}

