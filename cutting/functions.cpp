#include "functions.hpp"
#include "point.hpp"
#include "pointer.hpp"
#include "polygonc.hpp"
#include <iostream>

using namespace std;
// using namespace recti;

/** @brief used to do some pre-dealing to each rpolygon
 *  (a polygon whose angles are all rectangles, in lineda)
 *  in the input list<rpolygon>
 *  build Pn[] and corredponding Polygonc object to cut
 *  each rpolygon into rectangles
 *  results are put into vector<rectangle>
 *  used in main()
 */
void polygon_cutting(list<rpolygon> &rp, vector<rectangle> &R) {
  std::list<rpolygon>::iterator rit;

  for (rit = rp.begin(); rit != rp.end(); rit++) {
    rpolygon rpp = *rit;
    // remove duplicates of consecutive elements
    // remove duplicates of consecutive V/H segments
    // anti-clockwise order
    // the documentation of normalize() in polygon.hpp in lineda
    rpp.normalize();

    // fixed preparation to get the points of the rpolygon
    typedef RPolygon<int>::pt_set_type pt_set_type;
    typedef pt_set_type::const_iterator const_iterator;
    const pt_set_type pt_set = rpp.get_pt_set();
    const_iterator it = pt_set.begin();

    const size_t M = pt_set.num_nodes();
    // cout << "M is " << M << endl;
    // size_t number = M;

    std::vector<Pointer<int>> Pn(M, Pointer<int>(0, 0, 0, 0));

    // initialize Pn[]
    // cout << "now points of the polygon are" << endl;
    size_t i = 0;
    do {
      Pn[i] = Pointer<int>(it->x(), it->y(), 0, 0);
      // cout << "(" << it->x() << "," << it->y() << ")" << endl;
      Pn[i].prev(&Pn[i - 1]);
      Pn[i].next(&Pn[i + 1]);
      ++i;
    } while (++it != pt_set.begin());
    assert(i == M);
    Pn[0].prev(&Pn[i - 1]);
    Pn[i - 1].next(&Pn[0]);

    // initialize Polygonc and cut
    Polygonc<int> p(Pn, 0);
    p.cut();

    // turn rectangles in p._rectanglegroup[]
    // to rectangles in lineda
    for (int ii = 0; ii < p._rectanglenum; ii++) {
      Pointer<int> *node = p._rectanglegroup[ii];
      Pointer<int> *ll = node;
      // find the left-bottom point
      for (Pointer<int> *next = node->_next; next != node; next = next->_next)
        if (ll->_x > next->_x || ll->_x == next->_x && ll->_y > next->_y)
          ll = next;

      Pointer<int> *diagnode = ll->_next->_next;
      // 	  cout << "ll is: ";
      // 	  ll->print();
      // 	  cout << "diagnode is: ";
      // 	  diagnode->print();

      int width = diagnode->_x - ll->_x;
      int height = diagnode->_y - ll->_y;
      if (width == 0 || height == 0)
        continue;
      R.push_back(rectangle(point(ll->_x, ll->_y), width, height));
    }
  }
}
