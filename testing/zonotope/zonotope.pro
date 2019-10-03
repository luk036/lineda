TEMPLATE	= app
CONFIG		= warn_on debug
CONFIG          -= moc qt

INCLUDEPATH     += ../../../lineda/recti \
                   ../../../lineda/numeric \
                   ../../../lineda/dsl \
                   ../../../lineda/utility \
                   ../../../lineda/aaf

unix:LIBS       +=-L../../../lineda/lib -lrecti -lnumeric -ldsl -laaf

SOURCES		= Main.cpp
