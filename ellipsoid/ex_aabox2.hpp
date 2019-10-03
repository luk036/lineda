#ifndef _EX_AABOX2_HPP
#define _EX_AABOX2_HPP

#include <cassert>
#include <cmath>
#include <valarray>
#include <vector>

template <class Tp> class posynomial;
class AAF;

/**----------------------------------------------------------------------
 *
 *       AABOX2 VOLUME MAXIMIZATION EXAMPLE IN DENSE FORMAT
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
class ex_aabox2 {
  typedef std::valarray<double> Vec;

private:
  std::vector<posynomial<AAF> *> _P;

public:
  ex_aabox2();
  ~ex_aabox2();
  size_t num_constraints() const { return 6; }
  double operator()(const Vec &x, int j);
  void gradient(Vec &g, const Vec &x, int j);
};

#endif
