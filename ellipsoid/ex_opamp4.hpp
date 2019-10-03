#ifndef _EX_OPAMP4_HPP
#define _EX_OPAMP4_HPP

#include <valarray>
#include <aa.h>
#include <robust_GP.hpp>

/** Design variables */
enum {
  vL01, vW01, // transistor lengths and widths
  vL08, vW08,
  vL10, vW10,
  vL13, vW13,
  vIbias,     // bias current
  vA,         // 1st stage current coefficient
  vB,         // 2nd stage current coefficient
  NUM_VARS    // number of variables
};

/**
 *  This is an example taken from the thesis:
 *    Optimal Design of a CMOS Op-Amp via Geometric Programming 
 *    by Jia Hai-Long, and Tang Zhang-wen, in Fudan.
 */

template <typename _Tp>
class ex_opamp4 : public robust_GP<_Tp> {
  typedef robust_GP<_Tp> _Base;
  typedef std::valarray<double> Vec;
  
private:
  static double bound(double a) {return a;}
  static interval bound(const AAF& A) {return A.convert(); }

public:
  template <typename _Up>
  ex_opamp4(const std::valarray<_Up>& x);
  ~ex_opamp4();
};

#endif
