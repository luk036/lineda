cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
include_directories ( ../../dsl ../../utility )

set ( slicing_flrpln_HDRS
	
	)

set ( slicing_flrpln_SRCS
	Main.cpp
	)

add_executable ( slicing_flrpln ${slicing_flrpln_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
link_directories ( ../../lib )
target_link_libraries ( slicing_flrpln dsl )
