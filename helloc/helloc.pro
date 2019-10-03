TEMPLATE	= app
CONFIG		= warn_on debug thread
CONFIG          -= moc qt
QMAKE_CXXFLAGS += -DNDEBUG

debug {
  QMAKE_CXXFLAGS -= -DNDEBUG
}

### INCLUDEPATH     += ../dsl
### LIBS            += -lm

SOURCES		+= hello.c

