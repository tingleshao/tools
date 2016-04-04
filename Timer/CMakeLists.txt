cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)


add_definitions(-std=c++11 -fPIC)     #Use C++11 

#specifies a list of source files to be compiled
set( BASE_SOURCES 
   ATimer.cpp
)


#specifies the library to create. The name will be libbase_static.a and libbase.so in this case.
add_library( ATimer_static ${BASE_SOURCES})
add_library( ATimer SHARED ${BASE_SOURCES})

#install library files
install(TARGETS 
   ATimer
   ATimer_static
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   INCLUDES DESTINATION include
   PUBLIC_HEADER DESTINATION include
)

#install header files
install( FILES
   ATimer.h
   DESTINATION include
)
