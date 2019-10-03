#include "ex_sphere.hpp"
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <posynomial.hpp>
#include <valarray>
#include <vector>
using namespace std;

// Problem setup
template <typename _Tp>
template <typename _Up>
ex_sphere<_Tp>::ex_sphere(const std::valarray<_Up> &x) : _Base() {
  monomial<_Tp> X(NUM_VARS);
  monomial<_Tp> Y(NUM_VARS);
  monomial<_Tp> Z(NUM_VARS);

  X._a[0] = 1;
  Y._a[1] = 1;
  Z._a[2] = 1;

  posynomial<_Tp> p = (posynomial<_Tp>(X * X) + Y * Y + Z * Z) / 6.;
  _P.push_back(posynomial<_Tp>(1. / X / Y / Y / Z / Z / Z));
  _P.push_back(p);
  cout << "p = " << bound(exp(p(x))) << endl;
}

template <typename _Tp> ex_sphere<_Tp>::~ex_sphere() {}

// Explicit instantiation
#include <aa.h>
// xxx template class ex_sphere<double>;
template class ex_sphere<AAF>;
// xxx template ex_sphere<double>::ex_sphere(const std::valarray<double>& x);
template ex_sphere<AAF>::ex_sphere(const std::valarray<double> &x);
template ex_sphere<AAF>::ex_sphere(const std::valarray<AAF> &x);
