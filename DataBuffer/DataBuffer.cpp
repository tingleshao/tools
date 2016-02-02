#include <iostream>
#include <new>

#include "DataBuffer.h"

bool testDataBuffer()
{
   int elements = 100;

   //Create some buffers
   DataBuffer<uint8_t> uint8Buffer;
   DataBuffer<int16_t>int16Buffer;
   DataBuffer<int32_t>int32Buffer;
   DataBuffer<int64_t>int64Buffer;
   DataBuffer<double>DBuffer;
   DataBuffer<char>CBuffer;
   
   //Allocate buffers
   uint8Buffer.allocate(elements);
   int16Buffer.allocate(elements);
   int32Buffer.allocate(elements);
   int64Buffer.allocate(elements);
   DBuffer.allocate(elements);
   CBuffer.allocate(elements);

   uint8Buffer[1] = 1;
   int16Buffer[1] = -2;
   int32Buffer[1] = 3;
   int64Buffer[1] = -4;
   DBuffer[1] = -5.5;
   CBuffer[1] = 'A';


   std::cout << "uint8Buffer[1] 1:"<< (int)uint8Buffer.m_buffer[1] << endl;
   std::cout << "int16Buffer[1] -2:"<< int16Buffer.m_buffer[1] << endl;
   std::cout << "int32Buffer[1] 3:"<< int32Buffer.m_buffer[1] << endl;
   std::cout << "int64Buffer[1] -4:"<< int64Buffer.m_buffer[1] << endl;
   std::cout << "DBuffer[1] -5.5:"<< DBuffer.m_buffer[1] << endl;
   std::cout << "CBuffer[1] A:"<< CBuffer.m_buffer[1] << endl;

   //Deallocate buffers
   uint8Buffer.deallocate();
   int16Buffer.deallocate();
   int32Buffer.deallocate();
   int64Buffer.deallocate();
   DBuffer.deallocate();

   return true;
}

