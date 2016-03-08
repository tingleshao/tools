cmake_minimum_required(VERSION 2.8)

PROJECT(AquetiTools)

find_package(Threads REQUIRED)


#specifies a list of source files to be compiled
set( BASE_SOURCES 
   ATSQueueTest.cpp
)

if(MAKE_TESTS)
    add_executable(ATSQueue_test ${BASE_SOURCES})
    target_link_libraries(ATSQueue_test 
       ${CMAKE_THREAD_LIBS_INIT}
       ${CMAKE_THREAD_LIBS_INIT}               #For threaded operation
    )

    install(TARGETS 
        ATSQueue_test
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
        PUBLIC_HEADER DESTINATION include
        RUNTIME DESTINATION bin
    )
endif()

install(FILES
   ATSQueue.tcc
   DESTINATION include
)
