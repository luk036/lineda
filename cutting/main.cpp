#include "functions.hpp"
#include "point.hpp"
#include "rectangle.hpp"
#include "rpolygon.hpp"
#include "svgstream.hpp"
#include <iostream>
#include <list>
#include <vector>

using namespace std;
using namespace recti;

int main() {
  svgstream test("test.svg");

  //   point p[12] = { point(1,0), point(2,0), point(2,1), point(3,1),
  // 		  point(3,2), point(2,2), point(2,3), point(1,3),
  // 		  point(1,2), point(0,2), point(0,1), point(1,1)};
  point p[8] = {point(0, 0),   point(10, 0), point(10, 9), point(19, 9),
                point(19, 19), point(9, 19), point(9, 10), point(0, 10)};
  rpolygon rp1(p, 8);
  // make rpp change with the time
  srand((unsigned)time(NULL));
  rpolygon rpp = rp1.random(1000);
  // change the colour and fill of the rpolygon in output svg file
  rpp._svg_style = "fill:green;fill-opacity:0.5;stroke:black";
  // output the rpolygon
  test << rpp << "\n";

  // fixed preparation to get the points of the rpolygon
  typedef RPolygon<int>::pt_set_type pt_set_type;
  typedef pt_set_type::const_iterator const_iterator;
  const pt_set_type pt_set = rpp.get_pt_set();
  const_iterator it = pt_set.begin();

  cout << "points of the polygon input are" << endl;
  do {
    cout << "(" << it->x() << "," << it->y() << ")" << endl;
  } while (++it != pt_set.begin());

  list<rpolygon> rp;
  rp.push_back(rpp);
  vector<rectangle> R;
  polygon_cutting(rp, R);

  // output the resulting rectangles
  for (vector<rectangle>::iterator rectit = R.begin(); rectit != R.end();
       rectit++) {
    cout << *rectit << endl;
    rectit->_svg_style = "fill:red;fill-opacity:0.5;stroke:black";
    test << *rectit << "\n";
  }

  return EXIT_SUCCESS;
}
