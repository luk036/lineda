TEMPLATE	= app
CONFIG		= warn_on release
CONFIG          -= moc qt

INCLUDEPATH     += ../../dsl ../../utility
unix:LIBS       += -L../../lib -ldsl

SOURCES		= Main.cpp
