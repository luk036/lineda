TEMPLATE	= app
CONFIG		= warn_on debug thread
CONFIG          -= moc qt
QMAKE_CXXFLAGS += -DNDEBUG

debug {
  QMAKE_CXXFLAGS -= -DNDEBUG
}

### INCLUDEPATH     += ../dsl
### LIBS            += -lm

HEADERS		+= hello.hpp 
SOURCES		+= hello.cpp Main.cpp

