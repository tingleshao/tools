#Top-level cmake file for the XRayData Repository.
cmake_minimum_required(VERSION 2.8)

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

#Specify output locations for files.
set(INCLUDE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/include")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Add additional directories
include_directories(include ATimer)
include_directories(include ABuffer)
include_directories(include AThread)
include_directories(include ATSQueue)
include_directories(include ASocket)

#Set up compiler options
add_definitions(-std=c++11 -fPIC)     #Use C++11 

#setup into subdirectories to combile code
add_subdirectory(ATimer)
add_subdirectory(ABuffer)
add_subdirectory(AThread)
add_subdirectory(ATSQueue)
add_subdirectory(ASocket)

add_library( ATL SHARED
   ABuffer/DataBuffer.cpp
   ABuffer/ExtendedBuffer.cpp
   ASocket/BaseSocket.cpp
   ASocket/SocketServer.cpp
   AThread/AThread.cpp
   ATimer/ATimer.cpp
   ATSQueue/ThreadSafeQueue/ThreadSafeQueue.cpp
)

add_library( ATL_static STATIC 
   ABuffer/DataBuffer.cpp
   ABuffer/ExtendedBuffer.cpp
   ASocket/BaseSocket.cpp
   ASocket/SocketServer.cpp
   AThread/AThread.cpp
   ATimer/ATimer.cpp
   ATSQueue/ThreadSafeQueue/ThreadSafeQueue.cpp
)

add_subdirectory(test)


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
