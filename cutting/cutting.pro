

TEMPLATE = app
CONFIG -= moc
CONFIG += warn_on debug

INCLUDEPATH      += ./ ../lineda/recti ../lineda/dsl ../lineda/numeric ../lineda/utility

unix:LIBS        += -L../lineda/lib -lrecti -lnumeric -ldsl

#Headers
HEADERS          += pointer.hpp polygonc.hpp functions.hpp

# Input
SOURCES          += main.cpp functions.cpp pointer.cpp polygonc.cpp 
