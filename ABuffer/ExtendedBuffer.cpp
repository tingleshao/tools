#include <iostream>
#include "ExtendedBuffer.tcc"

using namespace std;
/**
 * \brief Unit test fuction for the ExtendedBuffer class
 **/
 bool testExtendedBuffer()
 {
    size_t elements = 100;
   
    //Create some buffers
    atl::ExtendedBuffer<uint16_t> uint16Buffer;

    //Create temp buffer
    std::vector<uint16_t> tempV;

    //Test setting elements
    for( int i = 0; i < elements; i++) {
       tempV.push_back(i);
    }
    uint16Buffer.setElements(  tempV, 0 );

    for( int i = 0; i < elements; i++ ) {
       if( tempV[i] != uint16Buffer[i] ) {
          std::cerr << "Extended Buffer failed to set elements from array" <<std::endl;
          return false;
       }
    }

    //Test setting by index;
    uint16Buffer[10] = 0;
    if( uint16Buffer[10] != 0 ) {
       std::cerr << "Extended Buffer failed to set elements from brackets" <<std::endl;
       return false;
    }
    uint16Buffer[10] = 10;

    //Append tempBuffer from elements/2
    uint16Buffer.setElements( tempV, elements/2);

    //Verify size
    if( uint16Buffer.getSize() != elements + elements/2) {
       std::cerr << "ExtendedBuffer size incorrect: "
                 << uint16Buffer.getSize() << " != "
                 << elements + elements/2 << std::endl;
       return false;
    }

    //Verify data
    for( int i =  0; i < uint16Buffer.getSize(); i++ ) {
       if( uint16Buffer[i] != i%(elements/2)) {
          if((( i < elements/2 )&&( uint16Buffer[i] != i ))||
             ( uint16Buffer[i] != i - elements/2)
            )

          {
             std::cerr<<"uint16Buffer["<<i<<"] = "
                   << uint16Buffer[i] << " not "
                   << i % (elements/2)
                   <<std::endl;
             return false;
          } 
       }
    }


   return true;
}
