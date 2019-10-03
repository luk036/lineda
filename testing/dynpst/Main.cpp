#include <intv_multiset.hpp>
#include <point.hpp>
#include <pst_tree.hpp>
#include <rectangle.hpp>
#include <svgstream.hpp>

using namespace std; // for intv_multiset
using namespace recti;

int main() {
  svgstream sout("dynpst.svg");
  intv_multiset<int> I;

  for (size_t i = 0; i < 2000; ++i) {
    point p = point::random();
    I.insert(p.x(), p.y());
    sout << p << '\n'; // draw the points in red
  }

  typedef intv_multiset<int>::iterator Iterator;
  Iterator it1 = I.lower_bound(-200, 150);
  Iterator it2 = I.upper_bound(300, 150);

  point::_svg_style = "fill:blue;stroke:black";
  for (; it1 != it2; ++it1) { // traversal the candidates
    sout << point(it1.x(), it1.y()) << '\n';
  }

  sout << rectangle(point(-200, -1200), // -1200=-Inf
                    point(300, 150))
       << '\n';
}
