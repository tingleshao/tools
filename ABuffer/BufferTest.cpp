/************************************************************
 * \file Unit test for buffer data
 ************************************************************/
#include <iostream>

#include "DataBuffer.h"
#include "ExtendedBuffer.tcc"
#include "TypeBuffer.tcc"

using namespace std;
using namespace atl;

int main( void ) 
{
   cout << "Buffer Test"<<endl;

   cout  <<"test DataBuffer"<<endl;
   if( !testDataBuffer()) {
      cerr << "Failed DataBuffer test"<<endl;
      return 0;
   }
   cout  <<"test TypeBuffer"<<endl;
   if( !testTypeBuffer()) {
      cerr << "Failed TypeBuffer test"<<endl;
      return 0;
   }
   cout  <<"test ExtendedBuffer"<<endl;
   if( !testExtendedBuffer()) {
      cerr << "Failed ExtendedBuffer test"<<endl;
      return 0;
   }


   cout << "All tests passed"<<endl;
   return 0;
}
