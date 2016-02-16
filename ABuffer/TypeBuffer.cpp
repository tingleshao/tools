#include <iostream>
#include "TypeBuffer.tcc"

using namespace std;

namespace atl
{
   /**
    * \brief Unit test fuction for the ExtendedBuffer class
    **/
   bool testTypeBuffer()
   {
      bool rc = true;
      size_t elements = 100;
   
      //Create some buffers
      TypeBuffer<uint8_t>  u8Buffer;
      TypeBuffer<int16_t>  i16Buffer;
      TypeBuffer<double>   DBuffer;
   
      //Allocate 100 items
      if( !u8Buffer.allocate(elements) ||
          !i16Buffer.allocate(elements) ||
          !DBuffer.allocate(elements)
        ) 
      {
         cerr <<  "Unable to allocate TypeBuffers"<<endl;
         return false;
      }
   
      //Assign values
      for( int i = 0; i < elements; i++ ) {
         u8Buffer[i]  = i;
         i16Buffer[i] = i - elements/2;
         DBuffer[i]   = (double)(i - elements/2)+.5;
      }
   
      //Compare values
      for( int i = 0; i < elements; i++ ) {
         if(( u8Buffer[i] != i )   || 
            ( i16Buffer[i] != i - elements/2) ||
            ( DBuffer[i]  != (double)(i - elements/2)+.5)) 
         {
            uint8_t res = u8Buffer[i];
              cerr << "Failed in comparison:\n" 
                   << "\tuint8:  "<< (uint8_t)res<<", "<< i  <<"\n"
                   << "\tint16:  "<< (int16_t)i16Buffer[i]<<", "<<(int16_t)(i - elements/2) <<"\n"
                   << "\tdouble: "<< (double)DBuffer[i] <<", "<<(double)(i - elements/2)+.5   << endl;
   
              rc = false;
              break;
         }
      }
   
      u8Buffer.deallocate();
      i16Buffer.deallocate();
      DBuffer.deallocate();
   
      return rc;
   }
}

