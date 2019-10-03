# Sample .pro file to illustrate the coding policy. The rules are
# based on the book "Large-Scale C++ Software Design,
# Addison-Wesley".
#
#  DEF - definition
#  MDR - major design rule
#  mdr - minor design rule
#  GL  - guideline
#  P   - principle

TEMPLATE	= lib
CONFIG		= warn_on debug thread
CONFIG          -= moc qt
unix:LIBS       += -lcppunit -ldl


# P: Cyclic physical dependencies among components inhibit
# understanding, testing, and reuse. (p.185)
# GL: Avoid cyclic physical dependencies among components. (p.185)
unix:LIBS       += -L../lib -lnumeric -lrecti
INCLUDEPATH     += . .. ../numeric ../recti ../utility


# mdr: The root names of the .c file and the .h file that comprise a
# component should match exactly (p.110)
HEADERS		+= rectangle_t.hpp \
                   rectangle.hpp \
                   snode.hpp
SOURCES		+= rectangle_t.cpp \
                   rectangle.cpp \
                   snode.cpp \
		   Main.cpp
INTERFACES	= 
