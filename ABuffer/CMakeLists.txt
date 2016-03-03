cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#Find OS Threads Library
find_package(Threads REQUIRED)

#This tells cmake to install header files when make install is executed
include(GenerateExportHeader)

#specifies a list of source files to be compiled
set( LIB_SOURCES 
   DataBuffer.cpp
#   ExtendedBuffer.cpp
#   TSArray.cpp
)

#specifies the library to create. The name will be libbase_static.a and libbase.so in this case.
add_library( ABuffer SHARED ${LIB_SOURCES})
add_library( ABuffer_static ${LIB_SOURCES})

set(ABuffer_INSTALL
    ABuffer
    ABuffer_static
)

#specifies an executable for testing
if(MAKE_TESTS)
    add_executable( bufferTest
       BufferTest.cpp
       ExtendedBuffer.cpp
       TSArray.cpp
       TSMatrix.cpp
    )

    target_link_libraries( bufferTest
       ABuffer_static
       ${CMAKE_THREAD_LIBS_INIT}
    )
    list(APPEND ABuffer_INSTALL bufferTest)
endif()

#install library files
install(TARGETS 
   ${ABuffer_INSTALL}
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
   RUNTIME DESTINATION bin
)

#install include files
install(FILES
   DataBuffer.h
   ExtendedBuffer.tcc
   TSArray.tcc
   TSMatrix.tcc
   DESTINATION include
)
