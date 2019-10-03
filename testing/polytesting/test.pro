TEMPLATE	= app
CONFIG		= warn_on release
CONFIG          -= moc qt

INCLUDEPATH     += ../../../lineda/dsl \
                   ../../../lineda/recti \
                   ../../../lineda/numeric \
                   ../../../lineda/utility

unix:LIBS       +=-L../../../lineda/lib -ldsl  -lrecti 

SOURCES		= main.cpp
