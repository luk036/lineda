TEMPLATE	= app
CONFIG		= warn_on debug
CONFIG          -= moc qt

QMAKE_CXXFLAGS += -DNDEBUG
debug {
  QMAKE_CXXFLAGS -= -DNDEBUG
}

INCLUDEPATH     += ../../utility
unix:LIBS       += -lgmpxx -lgmp

SOURCES		= Main.cpp
