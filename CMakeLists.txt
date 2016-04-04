#Top-level cmake file for the XRayData Repository.
cmake_minimum_required(VERSION 2.8.12)

PROJECT(ATL)

#Specify default installation directory (CMAKE_BINARY_DIR) that will be applied for make install
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 
   SET(CMAKE_INSTALL_PREFIX "/usr/local/CMakeExample" CACHE PATH "CMakeExample install prefix" FORCE) 
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 

#determine if we build documentation
find_package( Doxygen)
if(DOXYGEN_FOUND)
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
   add_custom_target(doc
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
   )
endif(DOXYGEN_FOUND)

#If we use clang, set up parameters
if( NOT CMAKE_COMPILER_SET )
   option(USE_CLANG "Use clang for compiler instead of gcc" ON)
   if( USE_CLANG)
      set(CMAKE_C_COMPILER "/usr/bin/clang")
      set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
      option(WITH_ASAN "Enable Address sanitizer" OFF)
   
      if( WITH_ASAN )
         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address -fno-optimize-sibling-calls -DVOGL_USE_STB_MALLOC=0")
         set(CMAKE_CXX_FLAGS_LIST "${CMAKE_CXX_FLAGS_LIST}  -fno-omit-frame-pointer -fsanitize=address -fno-optimize-sibling-calls -DVOGL_USE_STB_MALLOC=0")
         set(CMAKE_EXE_LINK_FLAGS_LIST "${CMAKE_EXE_LINK_FLAGS_LIST} -fsanitize=address -fno-omit-frame-pointer")
         set(CMAKE_SHARED_LINK_FLAGS_LIST "${CMAKE_SHARED_LINK_FLAGS_LIST} -fsanitize=address -fno-omit-frame-pointer")
      endif()
   endif()
   #This flag is set to indicate that the compiler has been specified. This is
   #necessary for using the compiler specified by a Superbuild
   set(CMAKE_COMPILER_SET ON CACHE BOOL "Indicated compiler is specified") 
endif( NOT CMAKE_COMPILER_SET)

#Option to make test executables
option(MAKE_TESTS "Build test executables" OFF)

# Additional build flags
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -O0") #Compile faster on debug, with warnings
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3") #Optimize compilation, no warnings
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2") #Else slightly optimize, with warnings
endif()

#Specify output locations for files.
set(INCLUDE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/include")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Add additional directories
include_directories(include Timer)
include_directories(include Buffer)
include_directories(include Image)
include_directories(include Thread)
include_directories(include TSQueue)
include_directories(include Socket)

set( ATL_Headers
   Buffer/BaseContainerMetadata.h
   Buffer/BaseContainerArrayMetadata.h
   Buffer/BaseContainerArray.h
   Image/ImageMetadata.h
   Buffer/BaseBuffer.h
   Buffer/BaseContainer.h
   Buffer/ExtendedBuffer.tcc
   Buffer/TSArray.tcc
   Buffer/TSMatrix.tcc
   Socket/BaseSocket.h
   Socket/SocketServer.h
   Thread/Thread.h
   Timer/Timer.h
   TSQueue/ATSQueue.tcc
)


#Set up compiler options
add_definitions(-std=c++11 -fPIC)     #Use C++11 

#create a list of files
set( SOURCE_FILES 
   Buffer/BaseContainerMetadata.cpp
   Buffer/BaseContainerArrayMetadata.cpp
   Buffer/BaseContainerArray.cpp
   Buffer/TSArray.cpp
   Buffer/BaseBuffer.cpp
   Buffer/BaseContainer.cpp
   Buffer/ExtendedBuffer.cpp
   Buffer/TSArray.cpp
   Image/ImageMetadata.cpp
   Socket/BaseSocket.cpp
   Socket/SocketServer.cpp
   Thread/Thread.cpp
   Timer/Timer.cpp
)

add_library( ATL SHARED
   ${SOURCE_FILES}
)

add_library( ATL_static STATIC 
   ${SOURCE_FILES}
)

if(MAKE_TESTS)
    add_subdirectory(test)
endif()

#install library files
install(TARGETS 
   ATL
   ATL_static
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
)

#Install the FindAtl.cmake file of a find directory is specified
install(FILES cmake/FindAtl.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)
install(FILES ${ATL_Headers} DESTINATION include )
