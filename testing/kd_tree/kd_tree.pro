TEMPLATE	= app
CONFIG		= warn_on debug
CONFIG          -= moc qt

INCLUDEPATH     += ../../cgl \
                   ../../dsl \
                   ../../recti \
                   ../../numeric \
                   ../../utility

unix:LIBS       += -L../../lib -lcgl -lrecti -ldsl -lnumeric

SOURCES		= Main.cpp
