#include <algorithm>
#include <fstream>
#include <iostream>
#include <pointer.hpp>
#include <polygonn.hpp>
#include <rectlink.hpp>
#include <sstream>

#include <rectangle.hpp>
#include <rpolygon.hpp>
#include <svgstream.hpp>
#include <vector>

using namespace recti;
using namespace dsl;
using namespace std;

int main(int argc, const char *argv[]) {
  const size_t N = (argc <= 1) ? 10 : atoi(argv[1]);
  srand((unsigned)time(NULL));
  RPolygon<int> P = RPolygon<int>::random(N);
  svgstream test("polycutting.svg");
  P.normalize();

  // xxx test << P;
  // xxx test << '\n';

  size_t i = 0;
  P.normalize();

  typedef RPolygon<int>::pt_set_type pt_set_type;
  typedef pt_set_type::const_iterator const_iterator;
  const pt_set_type pt_set = P.get_pt_set();
  const_iterator it = pt_set.begin();

  // xxx do { // traverse circulant linked list
  // xxx   if (it.next()->x() == it->x() && it.prev()->x() == it->x()) continue;
  // xxx   if (it.next()->y() == it->y() && it.prev()->y() == it->y()) continue;
  // xxx   ++i;
  // xxx } while (++it != pt_set.begin());

  const size_t M = pt_set.num_nodes();
  vector<Pointer<int>> Pn(M, Pointer<int>(0, 0, 0, 0));

  i = 0;
  do { // traverse circulant linked list
    // xxx if (it.next()->x() == it->x() && it.prev()->x() == it->x()) continue;
    // xxx if (it.next()->y() == it->y() && it.prev()->y() == it->y()) continue;
    Pn[i] = Pointer<int>(it->x(), it->y(), 0, 0);
    Pn[i].next(&Pn[i + 1]);
    Pn[i].prev(&Pn[i - 1]);
    ++i;
  } while (++it != pt_set.begin());

  assert(i == M);

  Pn[0].prev(&Pn[i - 1]);
  Pn[i - 1].next(&Pn[0]);

  Pointer<int> &a = Pn[0];

  Polygonn<int> p(&a, i, 0);

  Pointer<int> *curr = &a;
  curr = p.normalize(curr);
  cout << "next step" << endl;
  int o = p.area(&a);

  p.cut(curr);

  cout << "_rectanglenum=" << p._rectanglenum << endl;

  // xxx Pointer<int>* cr;
  for (int ii = 0; ii < p._rectanglenum; ii++) {
    Pointer<int> *node = p._rectanglegroup[ii];
    Pointer<int> *nextnode = node->_next;
    Pointer<int> *prevnode = node->_prev;

    int width = nextnode->_x - node->_x;
    int height = prevnode->_y - node->_y;

    // xxx double ratio = double(width)/height;

    // if (ratio >= 0.25 && ratio <= 4) {
    // xxx cout << cr->_x <<" "<<cr->_y<<endl;
    test << "<rect"
         << " x=\"" << node->_x << "\""
         << " y=\"" << node->_y << "\""
         << " width=\"" << width << "\""
         << " height=\"" << height << "\""
         << " style=\"stroke:black;fill:red;fill-opacity:0.3\""
         << "/>\n";
    // }
  }

  cout << "area=" << o << endl;
  p.cparea(o);

  p.enlarge(2);
}
