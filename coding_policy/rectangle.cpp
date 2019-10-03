// Sample .cpp file to illustrate the coding policy. The rules are
// based on the books "Large-Scale C++ Software Design" and "Effective
// C++, 2nd Ed"
//
//  DEF - definition
//  MDR - major design rule
//  mdr - minor design rule
//  GL  - guideline
//  P   - principle
//
//  effc++ - rules from Effective C++.
//

// MDR: The .c file of every component should include its own .h file as
// the first substantive line of code. (p.110)
// P: Latent usage errors can be avoided by ensuring that the .h file of
// a component parses by itself - without externally-provided
// declarations or definitions (p.111)
#include "rectangle.hpp"

// GL: Clients should include header files providing required type
// definitions directly; except for non-private inheritance, avoid
// relying on one header file to include another (p.113)
// GL: A component x should include y.h only if x makes direct substantive
// use of a class or free operator function defined in y. (p.135)
#include <point.hpp>   // fine
#include <polygon.hpp> // fine

#include <zwindow.hpp> // bad idea if "zwindow" depends on
                       // "rectangle"

// effc++: Prefer <iostream> to <stdio.h> (Item 2)
#include <iostream> // fine

// MDR: Avoid data with external linkage at file scope. (p.70)
int size;            // bad idea
double scale;        // bad idea
const char *system1; // bad idea
/** ??? */
struct Global {
  static int size;           // bad idea
  static double scale;       // bad idea
  static const char *system; // bad idea
};

// MDR: Avoid free functions (including operator functions) at file scope in
// .c files. (p. 72)
int get_monitor_resolution();                              // bad idea
void set_system_scale(double scale_factor);                // bad idea
int is_password_correct(const char *usr, const char *psw); // bad idea

// MDR: Avoid definitions with external linkage in the .c file of a
// component that are not declared explicitly in the corresponding .h
// file. (p.115)
extern double area(const recti::point &p1, const recti::point &p2,
                   const recti::point &p3); // bad idea

// MDR: Avoid accessing a definition with external linkage in another
// component via a local declaration; instead, include the .h file for
// that component. (p.119)
extern "C" double pow(double, double); // bad idea

// effc++: Prefer "const" to "#define (Item 1)
static const double Pi = 3.14159265358;

// effc++: Prefer "inline" to "#define" (Item 1)
template <class T> inline const T &max(const T &x, const T &y) {
  return x > y ? x : y;
}

namespace recti {
rectangle rectangle::_zero = rectangle(); // fine

// MDR: Logical entities declared within a component should not be
// defined outside that component. (p.108)
bool polygon::contain(const point &q) const { // bad idea
  // ...
}

// effc++: Prefer <iostream> to <stdio.h> (Item 2)
std::ostream &operator<<(std::ostream &os, const rectangle &r) { // fine
  os << "<rect"
     << " width=\"" << r.width() << "\""
     << " height=\"" << r.height() << "\""
     << " fill=\"none\" stroke=\"black\""
     << "/>";
  return os;
}
}
