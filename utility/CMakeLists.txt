cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
set ( LIBRARY_OUTPUT_PATH ../lib )
set ( utility_HDRS
	concept_test.hpp
	utility.hpp
	svgstream.hpp
	dotstream.hpp
	)

set ( utility_SRCS
	concept_test.cpp
	utility.cpp
	svgstream.cpp
	dotstream.cpp
	)

add_library ( utility ${utility_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( utility  )
