#ifndef _EX_OPAMP_HPP
#define _EX_OPAMP_HPP

#include <cassert>
#include <cmath>
#include <valarray>
#include <vector>

/**
 *   This is an example taken directly from the paper:
 *       Optimal Design of a CMOS Op-Amp via Geometric Programming
 *       by Hershenson, Boyd, and Lee.
 *       IEEE TCAD vol 20. no 1, 2001 p1-21
 */
class ex_opamp {
  typedef std::valarray<double> Vec;

private:
  size_t _M;                 // number of constraints;
  std::vector<size_t> _inds; // indices
  std::vector<Vec> _A;       // posynomial exponents
  Vec _b;                    // posynomial coefficients
  Vec _y;                    // record intermediate result

public:
  ex_opamp();
  ~ex_opamp() {}
  size_t num_constraints() const { return _M; }
  double operator()(const Vec &x, int j);
  void gradient(Vec &g, const Vec &x, int j);
};

#endif
