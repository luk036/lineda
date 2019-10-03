cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall -std=c++1z )
# find_package ( Qt4 REQUIRED )
# include ( ${QT_USE_FILE} )
# include_directories (
#	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
#	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
#	)
add_subdirectory( aaf )
add_subdirectory( ellipsoidAA )
#add_subdirectory( rgp_yalaa )
add_subdirectory( utility )
add_subdirectory( dsl )
add_subdirectory( numeric )
add_subdirectory( recti )
add_subdirectory( cgl )
add_subdirectory( cppunit )
#add_subdirectory( testing )
