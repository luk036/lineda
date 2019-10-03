cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
include_directories ( 
  .. ../numeric ../recti ../dsl ../utility ../cgl ../aaf
  )
set ( cppunit_HDRS
	kt_rectangle_t.hpp
	kd_tree_t.hpp
	vector2_t.hpp
	matrix2_t.hpp
	polynomial_t.hpp
	triple_t.hpp
	point_t.hpp
	path_t.hpp
	rectangle_t.hpp
	rpolygon_t.hpp
	segment_t.hpp
	orient_t.hpp
	zonotope_t.hpp
	bin_tree_t.hpp
	sclist_t.hpp
	slist_t.hpp
	snode_t.hpp
	dnode_t.hpp
	intv_set_t.hpp
	intv_map_t.hpp
	)

set ( cppunit_SRCS
	kt_rectangle_t.cpp
	kd_tree_t.cpp
	vector2_t.cpp
	matrix2_t.cpp
	polynomial_t.cpp
	triple_t.cpp
	point_t.cpp
	path_t.cpp
	rectangle_t.cpp
	rpolygon_t.cpp
	segment_t.cpp
	orient_t.cpp
	zonotope_t.cpp
	bin_tree_t.cpp
	sclist_t.cpp
	slist_t.cpp
	snode_t.cpp
	dnode_t.cpp
	Main.cpp
	intv_set_t.cpp
	intv_map_t.cpp
	)

add_executable ( cppunit2 ${cppunit_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
link_directories ( ../lib )
target_link_libraries ( cppunit2 cppunit dl cgl recti numeric dsl utility aaf )
#target_link_libraries ( cppunit2 cppunit dl aaf utility dsl numeric recti cgl )
