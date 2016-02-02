#Top-level cmake file for the XRayData Repository.
cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

#Set up compiler to c++11 standard
#add_definitions(-std=c++11)

#Specify default installation directory (CMAKE_BINARY_DIR) that will be applied for make install
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 
   SET(CMAKE_INSTALL_PREFIX "/usr/local/CMakeExample" CACHE PATH "CMakeExample install prefix" FORCE) 
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 

#Specify output locations for files.
set(INCLUDE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/include")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


#setup into subdirectories to combile code
add_subdirectory(Timer)
add_subdirectory(DataBuffer)
add_subdirectory(C11Thread)
