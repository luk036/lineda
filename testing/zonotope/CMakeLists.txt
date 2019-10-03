cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )
include_directories( ../../recti
                   ../../numeric
                   ../../dsl
                   ../../utility
		   ../../aaf
		   )
set ( zonotope_HDRS
	
	)

set ( zonotope_SRCS
	Main.cpp
	)

add_executable ( zonotope ${zonotope_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
link_directories ( ../../lib )
target_link_libraries ( zonotope recti numeric dsl aaf )
