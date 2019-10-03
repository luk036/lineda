#ifndef _EX_OPAMP2_HPP
#define _EX_OPAMP2_HPP

#include <valarray>
#include <aa.h>
#include "robust_GP.hpp"

/** Design variables */
enum { 
  W1, W3, W5, W6, W7, W8,     // widths of all transistors
  L1, L3, L5, L6,             // lengths of all transistors
  Cc,                         // value of compensation capacitor
  Ibias,                      // bias current
  T2, T3, T4,
  NUM_VARS,                   // number of variables
  W2=W1, W4=W3, L2=L1, L4=L3, // transitors M1=M2, M3=M4
  L7=L5, L8=L5                // biasing transistors M5, M7, and M8 must match
};

/**
 *   This is an example taken directly from the paper:
 *       Optimal Design of a CMOS Op-Amp via Geometric Programming
 *       by Hershenson, Boyd, and Lee.
 *       IEEE TCAD vol 20. no 1, 2001 p1-21
 */
template <typename _Tp>
class ex_opamp2 : public robust_GP<_Tp> {
  typedef robust_GP<_Tp> _Base;
  typedef std::valarray<double> Vec;
  
private:
  static double bound(double a) {return a;}
  static interval bound(const AAF& A) {return A.convert(); }

public:
  template <typename _Up>
  ex_opamp2(const std::valarray<_Up>& x);
  ~ex_opamp2();
};


#endif
