#ifndef _EX_SPHERE_HPP
#define _EX_SPHERE_HPP

#include <aa.h>
#include <robust_GP.hpp>
#include <valarray>

/** Design variables */
enum {
  vx,
  vy,
  vz,
  NUM_VARS // number of variables
};

template <typename _Tp> class ex_sphere : public robust_GP<_Tp> {
  typedef robust_GP<_Tp> _Base;
  typedef std::valarray<double> Vec;

private:
  static double bound(double a) { return a; }
  static interval bound(const AAF &A) { return A.convert(); }

public:
  template <typename _Up> ex_sphere(const std::valarray<_Up> &x);
  ~ex_sphere();
};

#endif
