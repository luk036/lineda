cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )
include_directories( ../aaf )
set ( ellipsoid_HDRS
	robust_GP.hpp
	ex_opamp2.hpp
	ellipsoid.hpp
	posynomial.hpp
	monomial.hpp
	)

set ( ellipsoid_SRCS
	testaaopamp.cpp
	robust_GP.cpp
	ex_opamp2.cpp
	posynomial.cpp
	monomial.cpp
	)

add_executable ( ellipsoid ${ellipsoid_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
link_directories( ../lib )
target_link_libraries ( ellipsoid aaf )
