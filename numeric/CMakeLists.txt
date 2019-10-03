cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
include_directories ( ../aaf )
set ( LIBRARY_OUTPUT_PATH ../lib )
set ( numeric_HDRS
	interval.hpp
	matrix2.hpp
	newton.hpp
	polynomial.hpp
	vector2.hpp
	triple.hpp
	)

set ( numeric_SRCS
	interval.cpp
	matrix2.cpp
	newton.cpp
	polynomial.cpp
	vector2.cpp
	triple.cpp
	interval_t.cpp
	)

add_library ( numeric ${numeric_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( numeric  )
