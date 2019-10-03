#ifndef _EX_BOX_HPP
#define _EX_BOX_HPP

#include <cassert>
#include <cmath>
#include <valarray>
#include <vector>

/**----------------------------------------------------------------------
 *
 *       BOX VOLUME MAXIMIZATION EXAMPLE IN DENSE FORMAT
 *
 *----------------------------------------------------------------------
 *
 *   maximize:   h^-1*w^-1*d^-1
 *   subject to: (2/Awall)*h*w+(2/Awall)*h*d <= 1,
 *               (1/Aflr)*w*d <= 1,
 *               alpha*h^-1*w <= 1,
 *               (1/beta)*h*w^-1 <= 1,
 *               gamma*w*d^-1 <= 1,
 *               (1/delta)*w^-1*d <= 1.
 *
 *   This is an example taken directly from the paper:
 *       A Tutorial on Geometric Programming (see pages 8 and 13)
 *       by Boyd, Kim, Vandenberghe, and Hassibi.
 *
 */
class ex_box {
  typedef std::valarray<double> Vec;

private:
  size_t _M;                 // number of constraints;
  std::vector<size_t> _inds; // indices
  std::vector<Vec> _A;       // posynomial exponents
  Vec _b;                    // posynomial coefficients
  Vec _y;                    // record intermediate result

public:
  ex_box();
  ~ex_box() {}
  size_t num_constraints() const { return _M; }
  double operator()(const Vec &x, int j);
  void gradient(Vec &g, const Vec &x, int j);
};

#endif
