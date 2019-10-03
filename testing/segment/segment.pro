TEMPLATE	= app
CONFIG		= warn_on debug
CONFIG          -= moc qt

INCLUDEPATH     += ../../../lineda/recti \
                   ../../../lineda/numeric \
                   ../../../lineda/utility

unix:LIBS       +=-L../../../lineda/lib -lrecti -lnumeric

SOURCES		= Main.cpp
