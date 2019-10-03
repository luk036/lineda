cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
set ( LIBRARY_OUTPUT_PATH ../lib )
include_directories (
  ../recti ../dsl ../numeric ../utility /usr/include/c++
  )
set ( cgl_HDRS
	sweeper.hpp
	pt_stree.hpp
	pt_bin_tree.hpp
	pt_tri_tree.hpp
	intvl_set.hpp
	overlap.hpp
	kd_tree.hpp
	kt_rectangle.hpp
	)

set ( cgl_SRCS
	sweeper.cpp
	pt_stree.cpp
	pt_bin_tree.cpp
	pt_tri_tree.cpp
	intvl_set.cpp
	overlap.cpp
	kd_tree.cpp
	)

add_library ( cgl ${cgl_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( cgl  )
