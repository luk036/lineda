TEMPLATE	= app
CONFIG		= warn_on debug
CONFIG          -= moc qt

INCLUDEPATH     += ../../../lineda/recti \
                   ../../../lineda/numeric \
                   ../../../lineda/dsl \
                   ../../../lindea/cgl \
                   ../../../lineda/utility

unix:LIBS       +=-L../../../lineda/lib -lcgl -lrecti -lnumeric -ldsl

SOURCES		= Main.cpp
