// Sample header file to illustrate the coding policy. The rules are
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

// MDR: Place a unique and predictable (internal) include guard around the
// contents of each header file (p.81)
#ifndef RECTI_RECTANGLE_HPP // fine: internal include guard
#define RECTI_RECTANGLE_HPP // fine

// mdr: Place a redundant (external) include guard around each preprocessor
// include directive in every header file. (p.85)
#ifndef RECTI_POINT_HPP // fine: redundant include guard
#include "point.hpp"    // fine
#endif                  // fine

#include <cassert> // fine
#include <iosfwd>  // fine

// MDR: Avoid using preprocessor macros in header files except as include
// guards. (p.75)
#define PI 3.14159265358                  // AVOID: macro constant
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y)) // AVOID: macro function

namespace recti { // fine

// MDR: Avoid enumerations, typedefs, and constants at file scope in .h
// files. (p.73)
extern int global_variable;     // AVOID: external data declaration
static int file_scope_variable; // AVOID: internal data declaration
const int BUFFER_SIZE = 256;    // AVOID: const data definition
enum Boolean { ZERO, ONE };     // AVOID: enumeration at file scope
typedef long Big_Int;           // AVOID: typedef at file scope

// MDR: Only classes, structures and free operator functions should be
// *declared* at file scope in a .h file (p.77)
class point;  // fine: class declaration
class driver; // fine: class declaration

/** MDR: only classes, structures,and inline (member or free
 * operator) functions should be
 * *defined* at file scope in a .h file. (p.77)
 */
class rectangle {
  enum Color { RED, GREEN };    // fine: enumeration in class scope
  typedef int (driver::*PMF)(); // fine: typedef in class scope
  friend class polygon;         // fine: friend declaration

  // GL: Avoid granting (long-distance) friendship to a logical entity
  // defined in another component. (p.137)
  // P: Granting (long-distance) friendship to a logical entity
  // defined in a separate physical part of the system violates the
  // encapsulation of the class granting that friendship. (p.140)
  friend class ZWindow; // AVOID: long-distance friendship;

  // mdr: Use all uppercase with underscore to identify immutable values
  // such as enumerators, "const" data, and pre-processor
  // constants. (p.92)
  enum Orient { // fine
    ROT_000,
    ROT_090,
    ROT_180,
    ROT_270,
    FLI_000,
    FLI_090,
    FLI_180,
    FLI_270
  };

public:
  // effc++: Avoid data members in the public interface (Item 20)
  point _lower_left;  // AVOID: public data member
  point _upper_right; // AVOID: public data member

private:
  // MDR: Keep class data members private. (p.65)
  // mdr: Use _ prefix to highlight class data members. (p.91)
  static rectangle _zero; // fine: static member declaration
  point _ll;              // fine: member data definition
  point _ur;              // fine: member data definition

public:
  rectangle() : _ll(0, 0), _ur(0, 0) {}

  // mdr: Never pass a user-defined type (i.e., class, struct) to a
  // function by value.
  // effc++: Prefer pass-by-reference to pass-by-value (Item 22)
  rectangle(const point &ll, const point &ur);

  // GL: Explicitly state (pre-)conditions under which behavior is
  // undefined. (p.89)
  /** Create a rectangle with lower left point, width and height.
      Pre-condition: width and height are non-negative.*/
  rectangle(const point &ll, int width, int height) : _ll(ll), _ur(ll) {
    // P: The use of assert statements can help to document the
    // assumptions you make when implementing your code. (p.90)
    // P. Specifying design decisions directly in the code instead
    // of relying on comments is a design goal.
    assert(width >= 0 && height >= 0);
    _ur._x += width;
    _ur._y += height;
  }

  static rectangle random();  // fine: static member function declaration
  inline double area() const; // fine: member function declaration

  const point &ll() const { return _ll; }            // fine
  const point &ur() const { return _ur; }            // fine
  int width() const { return ur().x() - ll().x(); }  // fine
  int height() const { return ur().y() - ll().y(); } // fine

  // MDR: Be consistent about identifier names; use underscore to delimit
  // words in identifiers.
  void rotate90_then_flip_y();

  // effc++: Use inlining judiciously (Item 33) [Note: You have to include
  // polygon.hpp in this header file if you plan to make the
  // following function inline.
  bool intersects(const polygon &P) const;
};

// effc++: Differentiate among member functions, non-member
// functions, and friend functions.
std::ostream &operator<<(std::ostream &os, const rectangle &r); // fine

inline double rectangle::area() const {
  return width() * (double)height();
} // fine: inline member function definition

// effc++: Don't try to return a reference when you must return an
// object (Item 23)
inline rectangle rectangle::random() {
  return rectangle(point::random(), rand() % 1000, rand() % 1000);
} // fine: inline static member function

inline bool operator==(const rectangle &lhs, const rectangle &rhs) {
  return lhs.ll() == rhs.ll() && lhs.ur() == rhs.ur();
} // fine: free inline operator
  // function definition

// MDR: Avoid free functions (except operator functions) at file scope in
// .h files. (p.72)
int min(int x, int y);       // AVOID: free function declaration
inline int max(int x, int y) // AVOID: free inline function
{                            //        definition
  return x > y ? x : y;
}
}

#endif
