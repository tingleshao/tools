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
    uint16Buffer.allocate(75, false);

    //Create temp buffer
    uint16_t buffer[elements];

    //Test setting elements
    for( size_t i = 0; i < elements; i++) {
       buffer[i] = i;
    }
    size_t count = uint16Buffer.setElements( buffer, elements, 0, true );
    if( count != elements ) {
       std::cerr <<"ExtendedBuffer failed to set elements. "<<count<<"!="<<elements<<std::endl;
       return false;
    }



    bool rc = true;
    for( size_t i = 0; i < elements; i++ ) {
       if( buffer[i] != uint16Buffer[i] ) {
          std::cerr << "Extended Buffer failed to set elements from array "<<i<<": "
                    << buffer[i] << "!="<< uint16Buffer[i] <<std::endl;
          rc = false;
       }
    }
    if( !rc ) {
       return rc;
    }

    //Test setting by index;
    uint16Buffer[10] = 0;
    if( uint16Buffer[10] != 0 ) {
       std::cerr << "Extended Buffer failed to set elements from brackets" <<std::endl;
       return false;
    }
    uint16Buffer[10] = 10;

    //Append tempBuffer from elements/2
    uint16Buffer.setElements( buffer, elements, elements/2, true );

    //Verify size
    if( uint16Buffer.getMaxIndex() != elements + elements/2) {
       std::cerr << "ExtendedBuffer size incorrect: "
                 << uint16Buffer.getSize() << " != "
                 << elements + elements/2 << std::endl;
       return false;
    }

    //Verify data
    for( size_t i =  0; i < uint16Buffer.getMaxIndex(); i++ ) {
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

    size_t currSize = uint16Buffer.getMaxIndex();
    size_t value = uint16Buffer.appendBuffer( uint16Buffer );
    if( value != currSize ) {
       std::cerr << "appendBuffer return did not match original maxIndex" 
                 << value <<"!="<<currSize <<std::endl;
       return false;
    }

    if( uint16Buffer.getMaxIndex() != 2*currSize ) {
       std::cerr << "Extended Buffer failed to to append: " 
                 << uint16Buffer.getMaxIndex()<<"!="<< 2*currSize  
                 << std::endl;
       return false;
    }

    for( size_t i = 0; i < currSize; i++ ) {
       if(  uint16Buffer[i] != uint16Buffer[i+currSize] ) {
          std::cerr << "AppendBuffer failed : "<<i<<" - " 
                    << uint16Buffer[i]<<"!="<< uint16Buffer[i+currSize]
                    << std::endl;
          rc = false;
       }
    }

    if( !rc ) {
       return false;
    }


   return true;
}
