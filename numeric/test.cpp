#include "concept_test.hpp"
#include "polynomial.hpp"
#include "rational.hpp"
#include <boost/concept_check.hpp>
#include <complex>

using namespace std;
using namespace boost;
using namespace std::rel_ops;

int main() {
  int i1(2), i2(-3), i3(4);

  test_equal_to(i1, i2, i3);
  test_not_equal_to(i1, i2, i3);
  test_addition(i1, i2, i3);
  test_subtraction(i1, i2, i3);
  test_multiplication(i1, i2, i3);
  test_division(i1, i2, i3);
  // xxx test_mod(i1, i2, i3);
  test_less(i1, i2, i3);

  double d1(2.0), d2(5.0), d3(-4.0);

  test_equal_to(d1, d2, d3);
  test_not_equal_to(d1, d2, d3);
  test_addition(d1, d2, d3);
  test_subtraction(d1, d2, d3);
  test_multiplication(d1, d2, d3);
  test_division2(d1, d2, d3);
  // No operator%() in double
  // xxx test_mod(d1, d2, d3);
  test_less(d1, d2, d3);

  complex<int> ci1(i1, i2), ci2(i2, i3), ci3(i3, i1);
  test_equal_to(ci1, ci2, ci3);
  test_not_equal_to(ci1, ci2, ci3);
  test_addition(ci1, ci2, ci3);
  test_subtraction(ci1, ci2, ci3);
  test_multiplication(ci1, ci2, ci3);
  test_division2(ci1, ci2, ci3);
  // No operator% in complex<>
  // test_mod(ci1, ci2, ci3);
  // complex<> is not LessThanComparable
  // test_less(ci1, ci2, ci3);

  complex<double> cd1(d1, d2), cd2(d2, d3), cd3(d3, d1);
  test_equal_to(cd1, cd2, cd3);
  test_not_equal_to(cd1, cd2, cd3);
  test_addition(cd1, cd2, cd3);
  test_subtraction(cd1, cd2, cd3);
  test_multiplication(cd1, cd2, cd3);
  test_division(cd1, cd2, cd3);
  cout << (cd1 / cd2) / cd3 << endl;
  cout << (cd1 / cd3) / cd2 << endl;
  // No operator% in complex<>
  // test_mod(cd1, cd2, cd3);
  // complex<> is not LessThanComparable

  complex<complex<int>> cci1(ci1, ci2), cci2(ci2, ci3), cci3(ci3, ci1);
  test_equal_to(cci1, cci2, cci3);
  test_not_equal_to(cci1, cci2, cci3);
  test_addition(cci1, cci2, cci3);
  test_subtraction(cci1, cci2, cci3);
  test_multiplication(cci1, cci2, cci3);
  test_division2(cci1, cci2, cci3);

  rational<int> ri1(3, 5), ri2(-4, 3), ri3(7, -2);

  test_equal_to(ri1, ri2, ri3);
  test_not_equal_to(ri1, ri2, ri3);
  test_addition(ri1, ri2, ri3);
  test_subtraction(ri1, ri2, ri3);
  test_multiplication(ri1, ri2, ri3);
  test_division2(ri1, ri2, ri3);
  // test_mod(ri1, ri2, ri3);
  test_less(ri1, ri2, ri3);

  complex<rational<int>> cri1(ri1, ri2), cri2(ri2, ri3), cri3(ri3, ri1);
  test_equal_to(cri1, cri2, cri3);
  test_not_equal_to(cri1, cri2, cri3);
  test_addition(cri1, cri2, cri3);
  test_subtraction(cri1, cri2, cri3);
  test_multiplication(cri1, cri2, cri3);
  test_division2(cri1, cri2, cri3);
  cout << cri1 << endl;

  // Porblem: no operator%() in complex<int>
  // rational<complex<int> >  rci1(ci1, ci2), rci2(ci2, ci3), rci3(ci3, ci1);

  double ad1[] = {1.0, 2.0, 3.0, 0.0, 5.0};
  double ad2[] = {0.0, 2.0, 4.0};
  double ad3[] = {3.0, 1.0};

  polynomial<double> pd1(ad1, 5), pd2(ad2, 3), pd3(ad3, 2);

  test_equal_to(pd1, pd2, pd3);
  test_not_equal_to(pd1, pd2, pd3);
  test_addition(pd1, pd2, pd3);
  test_subtraction(pd1, pd2, pd3);
  test_multiplication(pd1, pd2, pd3);
  test_division2(pd1, pd2, pd3);

  cout << pd2 << endl;
  cout << pd3 << endl;
  cout << (pd2 / pd3) << endl;
  cout << pd2 % pd3 << endl;

  test_mod(pd1, pd2, pd3);
  test_less(pd1, pd2, pd3);

  double x1[] = {1.0, 2.0, 5.0, 7.0, 9.0};
  double y1[] = {-1.0, 3.0, 4.0, 8.0, 4.0};
  double x2[] = {2.0, 5.0, 1.0, 7.0, 9.0};
  double y2[] = {3.0, 4.0, -1.0, 8.0, 4.0};

  polynomial<double> p1(x1, y1, 5);
  polynomial<double> p2(x2, y2, 5);

  cout << p1(5.0) << endl;

  cout << pd3 << endl;

  // rational<int> ri0;
  rational<int> ari1[] = {1, 2, 3, 0, 5};
  rational<int> ari2[] = {0, 2, 4};
  rational<int> ari3[] = {3, 1};

  polynomial<rational<int>> pri1(ari1, 5), pri2(ari2, 3), pri3(ari3, 2);
  test_equal_to(pri1, pri2, pri3);
  test_not_equal_to(pri1, pri2, pri3);
  test_addition(pri1, pri2, pri3);
  test_subtraction(pri1, pri2, pri3);
  test_multiplication(pri1, pri2, pri3);
  test_division2(pri1, pri2, pri3);
  test_mod(pri1, pri2, pri3);

  cout << pri1 << endl;
  cout << pri2 << endl;
  cout << pri3 << endl;

  // xxx rational<polynomial<rational<int> > >
  // xxx    rpri1(pri1, pri2), rpri2(pri2, pri3), rpri3(pri3, pri1);
  // xxx cout << rpri1 << endl;
  // xxx cout << rpri2 << endl;
  // xxx cout << rpri3 << endl;
  // xxx test_less(rpri1, rpri2, rpri3);
  // xxx test_equal_to(rpri1, rpri2, rpri3);
  // xxx test_not_equal_to(rpri1, rpri2, rpri3);
  // xxx cout << rpri1 + (rpri2 + rpri3) << endl;
  // xxx cout << (rpri1 + rpri2) + rpri3 << endl;
  // xxx
  // xxx test_addition(rpri1, rpri2, rpri3);
  // xxx test_subtraction(rpri1, rpri2, rpri3);
  // xxx test_multiplication(rpri1, rpri2, rpri3);
  // xxx test_division2(rpri1, rpri2, rpri3);
  // xxx test_mod(rpri1, rpri2, rpri3);
}
