cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )
include_directories( ../../recti
                   ../../numeric
                   ../../utility
		   )
set ( segment_HDRS
	
	)

set ( segment_SRCS
	Main.cpp
	)

add_executable ( segment ${segment_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
link_directories ( ../../lib )
target_link_libraries ( segment recti numeric )
