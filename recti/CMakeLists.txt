cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )
set ( LIBRARY_OUTPUT_PATH ../lib )
include_directories (
  ../dsl ../numeric ../utility ../aaf
  )
set ( recti_HDRS
	checker.hpp
	coord_sys.hpp
	geo_iter.hpp
	gpolygon.hpp
	grpolygon.hpp
	orient.hpp
	path.hpp
	point.hpp
	rectlink.hpp
	pointer.hpp
	point_set.hpp
	polygon.hpp
	polygonn.hpp
	rectangle.hpp
	)

set ( recti_SRCS
	checker.cpp
        coord_sys.cpp
        geo_iter.cpp
        gpolygon.cpp
        grpolygon.cpp
        orient.cpp
        rectlink.cpp
        path.cpp
        point.cpp
        pointer.cpp
        point_set.cpp
        polygon.cpp
        poly_rand.cpp
        polygonn.cpp
        rectangle.cpp
        rpolygon.cpp
        segment.cpp
        seg_overlap.cpp
        zonotope.cpp
	)

add_library ( recti ${recti_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( recti  )
