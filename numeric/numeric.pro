######################################################################
# Automatically generated by qmake (1.04a) Sat Jun 12 20:28:05 2004
######################################################################

TEMPLATE = lib
CONFIG += warn_on staticlib release
CONFIG -= qt moc
QMAKE_CXXFLAGS += -DNDEBUG
INCLUDEPATH += . ../aaf

debug {
  QMAKE_CXXFLAGS -= -DNDEBUG
}



# Input
HEADERS += interval.hpp \
           matrix2.hpp \
           newton.hpp \
           polynomial.hpp \
           vector2.hpp \
           triple.hpp
SOURCES += interval.cpp \
           matrix2.cpp \
           newton.cpp \
           polynomial.cpp \
           vector2.cpp \
           triple.cpp \
           interval_t.cpp

DESTDIR = ../lib
