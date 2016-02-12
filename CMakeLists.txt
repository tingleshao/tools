#Top-level cmake file for the XRayData Repository.
cmake_minimum_required(VERSION 2.8)

PROJECT(ATools)

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

#Specify output locations for files.
set(INCLUDE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/include")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Add additional directories
include_directories(include ATimer)
include_directories(include ABuffer)
include_directories(include AThread)
include_directories(include ThreadSafeQueue)
include_directories(include ASocket)

#Set up compiler options
add_definitions(-std=c++11)     #Use C++11 

#setup into subdirectories to combile code
add_subdirectory(ATimer)
add_subdirectory(ABuffer)
add_subdirectory(AThread)
add_subdirectory(ThreadSafeQueue)
add_subdirectory(ASocket)

link_directories(${CMAKE_BINARY_DIR}/lib)
add_library( ATools wrapper.cpp)

target_link_libraries( ATools
   ATimer
   ABuffer
   AThread
   ThreadSafeQueue
   ASocket
)

add_subdirectory(test)

#install library files
install(TARGETS 
   ATools
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
)





#add_library( AquetToolLib_static STATIC
#   ATimer
#   ABuffers
#   AThreads
#   ThreadSafeQueue
#   ASockets
#)
