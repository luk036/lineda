TEMPLATE	= app
CONFIG		= warn_on release
CONFIG          -= moc qt

QMAKE_CXXFLAGS += -DNDEBUG
debug {
  QMAKE_CXXFLAGS -= -DNDEBUG
}

INCLUDEPATH     += ../../dsl ../../utility
unix:LIBS       += -L../../lib -ldsl

SOURCES		= Main.cpp
