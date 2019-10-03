#include "pointer.hpp"
#include "polygonn.hpp"
#include "rectlink.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace recti;
using namespace dsl;
using namespace std;

int main() {
  cout << "firstpolygon" << endl;
  Pointer<int> a(10, 10, 6, 0, 0), b(10, 20, 6, 0, 0), c(20, 20, 6, 0, 0),
      d(20, 5, 6, 0, 0), e(15, 5, 6, 0, 0), f(15, 10, 6, 0, 0);

  Polygonn<int> p(&a, 6, 0);

  a.next(&f);
  a.prev(&b);
  b.next(&a);
  b.prev(&c);
  c.next(&b);
  c.prev(&d);
  d.next(&c);
  d.prev(&e);
  e.next(&d);
  e.prev(&f);
  f.next(&e);
  f.prev(&a);

  Pointer<int> *curr = &a;

  for (int i = 0; i < a._num; i++) {
    cout << curr->_x << "  " << curr->_y << endl;
    curr = curr->_prev;
  }

  cout << "next step" << endl;
  int o;
  o = p.area(&p);

  p.cut(&c);
  cout << "_rectanglenum=" << p._rectanglenum << endl;
  Pointer<int> *cr;
  for (int ii = 0; ii < p._rectanglenum; ii++)

  {
    cr = p._rectanglegroup[ii];
    cout << cr->_x << " " << cr->_y << endl;
  }

  p.cparea(o);
  p.enlarge(2);

  cout << "secondpolygon" << endl;
  Pointer<int> a1(10, 10, 10, 0, 0), b1(10, 20, 10, 0, 0), c1(20, 20, 10, 0, 0),
      d1(20, 30, 10, 0, 0), e1(30, 30, 10, 0, 0), f1(30, 20, 10, 0, 0),
      g1(50, 20, 10, 0, 0), h1(50, 0, 10, 0, 0), i1(40, 0, 10, 0, 0),
      j1(40, 10, 10, 0, 0);

  Polygonn<int> p1(&a1, 10, 0);

  a1.next(&j1);
  a1.prev(&b1);
  b1.next(&a1);
  b1.prev(&c1);
  c1.next(&b1);
  c1.prev(&d1);
  d1.next(&c1);
  d1.prev(&e1);
  e1.next(&d1);
  e1.prev(&f1);
  f1.next(&e1);
  f1.prev(&g1);
  g1.next(&f1);
  g1.prev(&h1);
  h1.next(&g1);
  h1.prev(&i1);
  i1.next(&h1);
  i1.prev(&j1);
  j1.next(&i1);
  j1.prev(&a1);

  Pointer<int> *curr1 = &a1;

  for (int i1 = 0; i1 < a1._num; i1++) {
    cout << curr1->_x << "  " << curr1->_y << endl;
    curr1 = curr1->_prev;
  }

  cout << "next step" << endl;
  int o1;
  o1 = p1.area(&p1);

  p1.cut(&c1);
  cout << "_rectanglenum=" << p1._rectanglenum << endl;
  Pointer<int> *cr1;
  for (int v = 0; v < p1._rectanglenum; v++)

  {
    cr1 = p1._rectanglegroup[v];
    cout << cr1->_x << " " << cr1->_y << endl;
  }

  p1.cparea(o1);
  cout << "thirdpolygon" << endl;
  Pointer<int> a11(10, 10, 10, 0, 0), b11(10, 40, 10, 0, 0),
      c11(30, 40, 10, 0, 0), d11(30, 30, 10, 0, 0), e11(20, 30, 10, 0, 0),
      f11(20, 20, 10, 0, 0), g11(40, 20, 10, 0, 0), h11(40, 0, 10, 0, 0),
      i11(30, 0, 10, 0, 0), j11(30, 10, 10, 0, 0);

  Polygonn<int> p11(&a11, 10, 0);

  a11.next(&j11);
  a11.prev(&b11);
  b11.next(&a11);
  b11.prev(&c11);
  c11.next(&b11);
  c11.prev(&d11);
  d11.next(&c11);
  d11.prev(&e11);
  e11.next(&d11);
  e11.prev(&f11);
  f11.next(&e11);
  f11.prev(&g11);
  g11.next(&f11);
  g11.prev(&h11);
  h11.next(&g11);
  h11.prev(&i11);
  i11.next(&h11);
  i11.prev(&j11);
  j11.next(&i11);
  j11.prev(&a11);

  Pointer<int> *curr11 = &a11;

  for (int i11 = 0; i11 < a11._num; i11++) {
    cout << curr11->_x << "  " << curr11->_y << endl;
    curr11 = curr11->_prev;
  }

  cout << "next step" << endl;
  int o11;
  o11 = p11.area(&p11);

  p11.cut(&c11);
  cout << "_rectanglenum=" << p11._rectanglenum << endl;
  Pointer<int> *cr11;
  for (int v1 = 0; v1 < p11._rectanglenum; v1++)

  {
    cr11 = p11._rectanglegroup[v1];
    cout << cr11->_x << " " << cr11->_y << endl;
  }

  p11.cparea(o11);

  cout << "fouthpolygon" << endl;
  Pointer<int> a111(20, 20, 12, 0, 0), b111(20, 40, 12, 0, 0),
      c111(30, 40, 12, 0, 0), d111(30, 50, 12, 0, 0), e111(50, 50, 12, 0, 0),
      f111(50, 30, 12, 0, 0), g111(40, 30, 12, 0, 0), h111(40, 10, 12, 0, 0),
      i111(60, 10, 12, 0, 0), j111(60, 0, 12, 0, 0), k111(30, 0, 12, 0, 0),
      l111(30, 20, 12, 0, 0);

  Polygonn<int> p111(&a111, 12, 0);

  a111.next(&l111);
  a111.prev(&b111);
  b111.next(&a111);
  b111.prev(&c111);
  c111.next(&b111);
  c111.prev(&d111);
  d111.next(&c111);
  d111.prev(&e111);
  e111.next(&d111);
  e111.prev(&f111);
  f111.next(&e111);
  f111.prev(&g111);
  g111.next(&f111);
  g111.prev(&h111);
  h111.next(&g111);
  h111.prev(&i111);
  i111.next(&h111);
  i111.prev(&j111);
  j111.next(&i111);
  j111.prev(&k111);
  k111.next(&j111);
  k111.prev(&l111);
  l111.next(&k111);
  l111.prev(&a111);
  Pointer<int> *curr111 = &a111;

  for (int i111 = 0; i111 < a111._num; i111++) {
    cout << curr111->_x << "  " << curr111->_y << endl;
    curr111 = curr111->_prev;
  }

  cout << "next step" << endl;
  int o111;
  o111 = p111.area(&p111);

  p111.cut(&c111);
  cout << "_rectanglenum=" << p111._rectanglenum << endl;
  Pointer<int> *cr111;
  for (int v11 = 0; v11 < p111._rectanglenum; v11++)

  {
    cr111 = p111._rectanglegroup[v11];
    cout << cr111->_x << " " << cr111->_y << endl;
  }

  p111.cparea(o111);
}
