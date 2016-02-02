#Top-level cmake file for the XRayData Repository.
cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

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

#Set up compiler options
add_definitions(-std=c++11)     #Use C++11 

#setup into subdirectories to combile code
add_subdirectory(Timer)
add_subdirectory(DataBuffer)
add_subdirectory(C11Thread)
add_subdirectory(test)

