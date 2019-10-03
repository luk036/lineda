#include "rgp_prob1.h"
#include "aaf/aa.h"

static const AAF              // test
    Awall = 100,              // area of wall
    Aflr = interval(8.0, 12), // area of floor
    alpha = 0.8,              // lowest aspect ratio
    beta = 1.25,              // highest aspect ratio
    gamma2 = 0.8,             // lowest aspect ratio
    delta = 1.25              // highest aspect ratio
    ;

enum vars {
  h, // height
  w, // weight
  d, // depth
  NUM_VARS
};

rgpprob1::rgpprob1() : rgp_base(NUM_VARS) {
  // Objective function: h^-1 w^-1 d^-1 (inverse of volume)
  {
    monomial<AAF> obj(NUM_VARS);
    obj._a[h] = -1;
    obj._a[w] = -1;
    obj._a[d] = -1;
    obj.set_coeff(1);
    rgp_base::_M.push_back(posynomial<AAF>(obj));
  }

  // (2/Awall)hw + (2/Awall)hd <= 1
  {
    monomial<AAF> T11(NUM_VARS);
    T11._a[h] = 1;
    T11._a[w] = 1;
    T11.set_coeff(2. / Awall);
    monomial<AAF> T12(NUM_VARS);
    T12._a[h] = 1;
    T12._a[d] = 1;
    T12.set_coeff(2. / Awall);
    posynomial<AAF> P1(T11);
    P1 += T12;
    rgp_base::_M.push_back(P1);
  }

  {
    monomial<AAF> T2(NUM_VARS);
    T2._a[w] = 1;
    T2._a[d] = 1;
    T2.set_coeff(1. / Aflr);
    rgp_base::_M.push_back(posynomial<AAF>(T2));
  }

  {
    monomial<AAF> T3(NUM_VARS);
    T3._a[h] = -1;
    T3._a[w] = 1;
    T3.set_coeff(alpha);
    rgp_base::_M.push_back(posynomial<AAF>(T3));
  }

  {
    monomial<AAF> T4(NUM_VARS);
    T4._a[h] = 1;
    T4._a[w] = -1;
    T4.set_coeff(1. / beta);
    rgp_base::_M.push_back(posynomial<AAF>(T4));
  }

  {
    monomial<AAF> T5(NUM_VARS);
    T5._a[w] = 1;
    T5._a[d] = -1;
    T5.set_coeff(gamma2);
    rgp_base::_M.push_back(posynomial<AAF>(T5));
  }

  {
    monomial<AAF> T6(NUM_VARS);
    T6._a[w] = -1;
    T6._a[d] = 1;
    T6.set_coeff(1. / delta);
    rgp_base::_M.push_back(posynomial<AAF>(T6));
  }
}
