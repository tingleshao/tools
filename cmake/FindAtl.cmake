# - Find ATL
# Find the ATL includes and client library
# This module defines
#  ATL_INCLUDE_DIR, where to find ATL/client/dbclient.h
#  ATL_LIBRARIES, the libraries needed to use ATL.
#  ATL_FOUND, If false, do not try to use ATL.

if(ATL_INCLUDE_DIR AND ATL_LIBRARIES)
   set(ATL_FOUND TRUE)

else(ATL_INCLUDE_DIR AND ATL_LIBRARIES)
  find_path(ATL_INCLUDE_DIR ATimer.h
      ${CMAKE_INCLUDE_PATH}
      include
      /usr/include/
      /usr/local/include/
      /usr/include/ATL/
      /usr/local/include/ATL/
	  /opt/ATL/include/
      $ENV{ProgramFiles}/ATL/*/include
      $ENV{SystemDrive}/ATL/*/include
      )

if(WIN32)
  find_library(ATL_LIBRARIES NAMES ATL
      PATHS
      $ENV{ProgramFiles}/ATL/*/lib
      $ENV{SystemDrive}/ATL/*/lib
      )
else(WIN32)
   message("MSG: Looking at ${CMAKE_LIB_PATH}")
  find_library(ATL_LIBRARIES NAMES ATL_static
      PATHS
      ${CMAKE_LIB_PATH}
      /usr/lib
      /usr/lib/ATL
      /usr/local/lib
      /usr/local/lib/ATL
	   /opt/ATL/lib
      )
endif(WIN32)

  if(ATL_INCLUDE_DIR AND ATL_LIBRARIES)
    set(ATL_FOUND TRUE)
    message(STATUS "Found ATL: ${ATL_INCLUDE_DIR}, ${ATL_LIBRARIES}")
  else(ATL_INCLUDE_DIR AND ATL_LIBRARIES)
    set(ATL_FOUND FALSE)
    if (ATL_FIND_REQUIRED)
		message(FATAL_ERROR "ATL not found.")
	else (ATL_FIND_REQUIRED)
		message(STATUS "ATL not found.")
	endif (ATL_FIND_REQUIRED)
  endif(ATL_INCLUDE_DIR AND ATL_LIBRARIES)

  mark_as_advanced(ATL_INCLUDE_DIR ATL_LIBRARIES)

endif(ATL_INCLUDE_DIR AND ATL_LIBRARIES)

