#include <iostream>
#include <thread>
#include "TSArray.tcc"

using namespace std;
using namespace atl;
 
size_t testSize = 100;
const size_t threadCount  = 100;

TSArray<double> tsa;

/**
 * \brief consumer thread for the TSAArray test function
 **/
void consumerThread( double id) 
{
   for( size_t i = 0; i < testSize; i++ ) {
      tsa.setItem( id, i);
      double value;
      tsa.getItem( &value, i);
   }
}
   
/**
 * \brief Unit test fuction for the ExtendedBuffer class
 **/
bool testTSArray()
{

   //Try to get an empty array. Should return 0
   size_t sz = tsa.getSize();
   if( sz != 0 ) {
      cout << "TSArray size = "<<sz<<" not 0!"<<endl;
      return false;
   }

   //Set size to 100. Make sure we get it
   tsa.setSize(testSize);
   sz = tsa.getSize();
   if( sz != testSize) {
      cout << "TSArray size = "<<sz<<" not "<<testSize<<"!"<<endl;
      return false;
   }

    
   double value = 2.2;
   tsa.setItem( value, 99 );
   double result;

   tsa.getItem( &result, 99 );
   if( result != value ) {
      cout << "TSArray value = "<<result<<" not "<<value<<endl;
      return false;
   }

/*
   std::thread t[threadCount];

   //Spawn a bunch of threads
   for( size_t i = 0; i < threadCount; i++ ) {
      t[i] = std::thread(consumerThread, (double)i);
   }

   for( size_t i = 0; i < threadCount; i++ ) {
      t[i].join();
   }
*/
   sz = tsa.getSize();
   tsa.push_back(44);
   size_t sz2 = tsa.getSize();
   if( sz2 != sz+1) {
      std::cout<<"TSArray failed to push item (size does not match):"<<sz2<<"!="<<sz<<std::endl;
      return false;
   }

   sz2 = tsa[sz];
   if( sz2 != 44) {
      std::cout<<"TSArray failed to push item. Unexpected value:"<<sz2<<"!="<<44<<std::endl;
      return false;
   }
   

   
   return true;
}
