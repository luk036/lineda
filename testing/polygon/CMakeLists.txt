cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )
include_directories( ../../recti
                   ../../numeric
                   ../../dsl
                   ../../utility
		   )

set ( polygon_HDRS
	
	)

set ( polygon_SRCS
	Main.cpp
	)

add_executable ( polygon ${polygon_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
link_directories ( ../../lib )
target_link_libraries ( polygon recti numeric dsl )
