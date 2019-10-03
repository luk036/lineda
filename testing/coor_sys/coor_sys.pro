TEMPLATE	= app
CONFIG		= warn_on release
CONFIG          -= moc qt

INCLUDEPATH     += ../../../lineda/recti \
                   ../../../lineda/numeric \
                   ../../../lineda/dsl \
                   ../../../lineda/utility

unix:LIBS       +=-L../../../lineda/lib -lrecti -lnumeric -ldsl

SOURCES		=main.cpp
