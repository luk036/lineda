TEMPLATE	= app
CONFIG		+= warn_on thread
CONFIG          -= moc qt
QMAKE_CXXFLAGS += -DNDEBUG

debug {
  QMAKE_CXXFLAGS -= -DNDEBUG
}

win32-msvc:LIBS			= ../../lib/cppunit.lib ../../lib/qttestrunner.lib
win32-msvc:TMAKE_CXXFLAGS	= /GX /GR
win32-msvc:DEFINES		= QT_DLL QTTESTRUNNER_DLL

unix:LIBS       += -lcppunit -ldl

INCLUDEPATH     += .. ../numeric ../recti ../dsl ../utility ../cgl ../aaf
unix:LIBS       += -L../lib -lcgl -lrecti -lnumeric -ldsl -lutility -laaf

HEADERS		+= kt_rectangle_t.hpp \
                   kd_tree_t.hpp \
                   interval_t.hpp \
                   vector2_t.hpp \
                   matrix2_t.hpp \
                   polynomial_t.hpp \
                   triple_t.hpp \
                   point_t.hpp \
                   path_t.hpp \
                   rectangle_t.hpp \
                   rpolygon_t.hpp \
                   segment_t.hpp \
                   orient_t.hpp \
                   zonotope_t.hpp \
                   bin_tree_t.hpp \
                   sclist_t.hpp \
                   slist_t.hpp \
                   snode_t.hpp \
                   dnode_t.hpp 

SOURCES		+= kt_rectangle_t.cpp \
                   kd_tree_t.cpp \
                   interval_t.cpp \
                   vector2_t.cpp \
                   matrix2_t.cpp \
                   polynomial_t.cpp \
                   triple_t.cpp \
                   point_t.cpp \
                   path_t.cpp \
                   rectangle_t.cpp \
                   rpolygon_t.cpp \
                   segment_t.cpp \
                   orient_t.cpp \
                   zonotope_t.cpp \
                   bin_tree_t.cpp \
                   sclist_t.cpp \
                   slist_t.cpp \
                   snode_t.cpp \
                   dnode_t.cpp \
		   Main.cpp
INTERFACES	= 

HEADERS         += intv_set_t.hpp intv_map_t.hpp
SOURCES         += intv_set_t.cpp intv_map_t.cpp
