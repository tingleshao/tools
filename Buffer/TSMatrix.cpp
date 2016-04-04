#include <iostream>
#include <thread>
#include "TSMatrix.tcc"

using namespace std;
using namespace atl;
 
size_t TSMTestSize = 100;
const size_t TSMThreadCount  = 100;

TSMatrix<double> tsm;

void TSMconsumerThread( double id) 
{
   for( size_t i = 0; i < TSMTestSize; i++ ) {
      tsm.setItem( id, {i%10, i%10, i%10});
      double value;
      tsm.getItem( &value, {i%10, i%10, i%10});
   }
}

   
/**
 * \brief Unit test fuction for the ExtendedBuffer class
 **/
bool testTSMatrix()
{
   std::vector<size_t> dims = tsm.getDimensions();
   if( dims.size() > 0 ) {
      cout << "TSMatrix size "<<dims.size()<<" not 0"<<endl;
      return false;
   }

   dims.assign( {10,10,10});
   tsm.setDimensions(dims);

   dims = tsm.getDimensions();
   if( dims.size() == 0 ) {
      cout << "Dimension size is 0"<<endl;
      return false;
   }
   if( dims[0] * dims[1] * dims[2] != 10*10*10) {
      cout << "TSMatrix dims:\n"
           << "\t"<<dims[0]
           << "\t"<<dims[1]
           << "\t"<<dims[2]
           << endl;
      return false;
   }
    
   std::vector<size_t> target = {5,5,5};
   double value = 2.2;
   tsm.setItem( value, target );
   double result;

   tsm.getItem( &result, target );
   if( result != value ) {
      cout << "TSMatrix value = "<<result<<" not "<<value<<endl;
      return false;
   }

   tsm.setItem( 3.0, {2,2,2});


   std::thread t[TSMThreadCount];

   //Spawn a bunch of threads
   for( uint16_t i = 0; i < TSMThreadCount; i++ ) {
      t[i] = std::thread(TSMconsumerThread, (double)i);
   }

   for( uint16_t i = 0; i < TSMThreadCount; i++ ) {
      t[i].join();
   }
   return true;
}
