= lib2xml =

A simple lex&yacc program that translates synopsys's .lib format
into an XML-like format.

To compile the program:
  
    $ qmake -project
    $ qmake
    $ make

To run a demo:

    $ ./lib2xml < power_sample.lib > test.xml



