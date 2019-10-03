cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
include_directories ( ../utility )
set ( LIBRARY_OUTPUT_PATH ../lib )
set ( dsl_HDRS
	sclist.hpp
	snode.hpp
	sl_base.hpp
	slist.hpp
	stree.hpp
	dnode.hpp
	dl_base.hpp
	dclist.hpp
	bt_node.hpp
	bin_tree.hpp
	tt_node.hpp
	tri_tree.hpp
	slicing_flrpln.hpp
	pst_tree.hpp
	intv_set.hpp
	intv_map.hpp
	intv_multiset.hpp
	intv_multimap.hpp
	)

set ( dsl_SRCS
	sclist.cpp
	snode.cpp
	sl_base.cpp
	slist.cpp
	stree.cpp
	dnode.cpp
	dl_base.cpp
	dclist.cpp
	bt_node.cpp
	bin_tree.cpp
	tt_node.cpp
	tri_tree.cpp
	slicing_flrpln.cpp
	pst_tree.cpp
	intv_set.cpp
	intv_map.cpp
	intv_multiset.cpp
	intv_multimap.cpp
	)

add_library ( dsl ${dsl_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( dsl  )
