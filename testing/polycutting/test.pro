TEMPLATE	= app
CONFIG		= warn_on 
CONFIG          -= moc qt

INCLUDEPATH     += ../../../lineda/dsl \
                   ../../../lineda/recti \
                   ../../../lineda/numeric \
                   ../../../lineda/utility

unix:LIBS       +=-L../../../lineda/lib -lrecti -ldsl -lnumeric 

SOURCES		= main.cpp
