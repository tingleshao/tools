cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#Set compiler/linker options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexpensive-optimizations")


#This tells cmake to install header files when make install is executed
include(GenerateExportHeader)

#specifies a list of source files to be compiled
set( LIB_SOURCES 
   DataBuffer.cpp
   ExtendedBuffer.cpp
)

#specifies the library to create. The name will be libbase_static.a and libbase.so in this case.
add_library( DataBuffer SHARED ${LIB_SOURCES})
add_library( DataBuffer_static ${LIB_SOURCES})

#specifies an executable for testing
add_executable( bufferTest
   BufferTest.cpp
   TypeBuffer.cpp
)

target_link_libraries( bufferTest
   DataBuffer_static
)




#install library files
install(TARGETS 
   bufferTest
   DataBuffer
   DataBuffer_static
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
   RUNTIME DESTINATION bin
)

#install include files
install(FILES
   DataBuffer.h
   ExtendedBuffer.h
   TypeBuffer.h
   DESTINATION include
)
