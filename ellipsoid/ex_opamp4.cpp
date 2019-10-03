#include "ex_opamp4.hpp"
#include <aa.h>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <posynomial.hpp>
#include <valarray>
#include <vector>
using namespace std;

// Constants
static const double PI = 3.141592654,
                    KT = 1.38e-23 * 300; // KBOLT * TEMPERATURE

// Specifications
static const double 
    Vdd = 3.3,      // positive supply voltage (V)
    Vss = 0.0,      // negative supply voltage (V)
    Cload = 3e-12,  // loading capacitance (F)

    Lmax = 2e-6,    // max device length (m)
    Lmin = 1e-6,    // min device length (m)
    Wmax = 1000e-6, // max device width  (m)
    Wmin = 2e-6,    // min device width  (m)
    VODmax = 0.3,   // overdrive voltage (V)
    VODmin = 0.1,   // overdrive voltage (V)

    VCMmax = 0.45 * Vdd, // max common-mode input voltage (V)
    VCMmin = 0.55 * Vdd, // min common-mode input voltage (V)
    VOUTmax = 0.9 * Vdd, // min output voltage (V)
    VOUTmin = 0.1 * Vdd, // max output voltage (V)

    AREAmax = 1e-8,  // maximum area (m^2)
    POWERmax = 5e-3, // max quiescent power (W)
    OLGmin = 80,     // min open-loop gain (dB)
    UGBmin = 50e6,   // min unit gain bandwidth (Hz)
    PMmin = 60,      // min phase margin (degree)

    CMRRmin = 75,  // min common-mode rejection ratio (dB)
    PPSRRmin = 80, // min pos. power supply rejection ratio (dB)
    NPSRRmin = 80, // min neg. power supply rejection ratio (dB)

    SRmin = 50e6,   // min slew rate (V/s)
    IRSNmax = 1e-6; // input-referred spot noise, 1kHz (V/sqrt(Hz))

// HSPICE Level 1 parameters
// xxx static const double
static const AAF TOX = interval(18.8e-9, 21.2e-9),
                 // xxx TOX = 20e-9,              // Oxide thickness (m)
    COX = 34.53e-12 / TOX, // oxide capacitance per unit area (F/m^2)
    LD = 0.2e-6,           // Lateral diffusion (m)

    VTN = interval(0.61, 0.79), // NMOS threshold voltage (V)
    // xxx VTN     = 0.7,            // NMOS threshold voltage (V)
    UN = 600e-4,   // NMOS carrier mobility (m^2/(Vs))
    KN = UN * COX, // NMOS intrinsic transconductance parameter (A/V^2)

    LAMBDAN = 0.03, // NMOS channel length modulation (V^-1)
    KFN = 4e-24,    // NMOS flicker noise coefficient
    KAN = 1,        // NMOS flicker noise exponent

    CGDON = 692e-12, // NMOS gate-grain overlap capacitance (F/m)
    CGSON = 692e-12, // NMOS gate-source overlap capacitance (F/m)
    CJN = 271e-6,    // NMOS zero-bias bulk junction capacitace (F/m^2)
    CJSWN = 0.6e-9,  // NMOS zero-bias sidewall bulk junction capacitance (F/m)

    GAMMAN = 0.298, // NMOS body effect factor (V^0.5)
    PHIN = 0.688,   // NMOS surface inversion potential
    RSHN = 40,      // NMOS diffusion sheet resistance (ohm per unit area)

    PBN = 0.904, // NMOS bulk junction contact potential (V)
    MJN = 0.5,   // NMOS bulk junction grading coefficient (1)
    MJSWN = 0.5, // NMOS bulk junction sidewall bulk grading coefficient

    VTP = interval(-0.99, -0.81), // PMOS threshold voltage (V)
    // xxx VTP     = -0.9,           // PMOS threshold voltage (V)
    UP = 200e-4,   // PMOS carrier mobility (m^2/(Vs))
    KP = UP * COX, // PMOS intrinsic transconductance parameter (A/V^2)

    LAMBDAP = 0.06, // PMOS channel length modulation (V^-1)
    KFP = 2e-24,    // PMOS flicker noise coefficient
    KAP = 1,        // PMOS flicker noise exponent

    CGDOP = 692e-12, // PMOS gate-grain overlap capacitance (F/m)
    CGSOP = 692e-12, // PMOS gate-source overlap capacitance (F/m)
    CJP = 423e-6,    // PMOS zero-bias bulk junction capacitace (F/m^2)
    CJSWP = 1.2e-9,  // PMOS zero-bias sidewall bulk junction capacitance (F/m)

    GAMMAP = 0.471, // PMOS body effect factor (V^0.5)
    PHIP = 0.730,   // PMOS surface inversion potential
    RSHP = 60,      // PMOS diffusion sheet resistance (ohm per unit area)

    PBP = 0.928, // PMOS bulk junction contact potential (V)
    MJP = 0.5,   // PMOS bulk junction grading coefficient (1)
    MJSWP = 0.5; // PMOS bulk junction sidewall bulk grading coefficient

// Problem setup
template <typename _Tp>
template <typename _Up>
ex_opamp4<_Tp>::ex_opamp4(const std::valarray<_Up> &x) : _Base() {
  _Base::_P.push_back(posynomial<_Tp>(NUM_VARS, 1)); // preserve for obj. fct.
  posynomial<_Tp> p(NUM_VARS, 1);

  size_t j = 0;

  // compensate capacitor and coefficient
  double Cc = Cload / 3.0;
  double Fc = Cc / (Cc + Cload);

  // current and current coefficients
  monomial<_Tp> Ibias(NUM_VARS);
  monomial<_Tp> A(NUM_VARS);
  monomial<_Tp> B(NUM_VARS);
  Ibias._a[vIbias] = 1.0;
  A._a[vA] = 1.0;
  B._a[vB] = 1.0;

  // transistor lengths and widths
  vector<monomial<_Tp>> L(15, monomial<_Tp>(NUM_VARS));
  vector<monomial<_Tp>> W(15, monomial<_Tp>(NUM_VARS));
  L[1]._a[vL01] = 1.0;
  W[1]._a[vW01] = 1.0;
  L[8]._a[vL08] = 1.0;
  W[8]._a[vW08] = 1.0;
  L[10]._a[vL10] = 1.0;
  W[10]._a[vW10] = 1.0;
  L[13]._a[vL13] = 1.0;
  W[13]._a[vW13] = 1.0;
  L[2] = L[1];
  W[2] = W[1];
  L[3] = L[13];
  W[3] = W[13] * A * 0.5;
  L[4] = L[13];
  W[4] = W[13] * A * 0.5;
  L[5] = L[8];
  W[5] = W[8] * A;
  L[6] = L[13];
  W[6] = W[13] * B;
  L[7] = L[8];
  W[7] = W[8] * B;
  L[9] = L[8];
  W[9] = W[8];
  L[11] = L[10];
  W[11] = W[10];
  L[12] = L[13];
  W[12] = W[13] * 4.0;
  L[14] = L[10];
  W[14] = sqrt(W[10] * W[13] * L[10] / L[13]) * B * Fc;

  // mobility of NMOS and PMOS
  // xxx vector<double> K(15, 0.0);
  vector<AAF> K(15, 0.0);
  K[1] = KP;
  K[2] = KP;
  K[3] = KN;
  K[4] = KN;
  K[5] = KP;
  K[6] = KN;
  K[7] = KP;
  K[8] = KP;
  K[9] = KP;
  K[10] = KN;
  K[11] = KN;
  K[12] = KN;
  K[13] = KN;
  K[14] = KN;

  // current
  vector<monomial<_Tp>> I(14, monomial<_Tp>(NUM_VARS));
  I[1] = Ibias * A * 0.5;
  I[2] = Ibias * A * 0.5;
  I[3] = Ibias * A * 0.5;
  I[4] = Ibias * A * 0.5;
  I[5] = Ibias * A;
  I[6] = Ibias * B;
  I[7] = Ibias * B;
  I[8] = Ibias;
  I[9] = Ibias;
  I[10] = Ibias;
  I[11] = Ibias;
  I[12] = Ibias;
  I[13] = Ibias;

  // over drive voltage
  vector<monomial<_Tp>> VOD(14, monomial<_Tp>(NUM_VARS));
  for (j = 1; j <= 13; ++j) {
    VOD[j] = sqrt(2.0 / K[j] * I[j] / W[j] * L[j]);
  }

  // trans-conductor: Gm
  vector<monomial<_Tp>> Gm(14, monomial<_Tp>(NUM_VARS));
  for (j = 1; j <= 13; ++j) {
    Gm[j] = sqrt(2.0 * K[j] * I[j] * W[j] / L[j]);
  }

  // gate-drain capacitor: Cgd
  vector<monomial<_Tp>> Cgd(15, monomial<_Tp>(NUM_VARS));
  for (j = 1; j <= 14; ++j) {
    Cgd[j] = W[j] * COX * LD;
  }

  // gate-source capacitor: Cgs
  vector<posynomial<_Tp>> Cgs(15, posynomial<_Tp>(NUM_VARS, 2));
  for (j = 1; j <= 14; ++j) {
    Cgs[j] = posynomial<_Tp>(W[j] * L[j] * COX * 2.0 / 3.0) + W[j] * LD * COX;
  }

  // Objective: minimize area
  p = posynomial<_Tp>(NUM_VARS, 14);
  for (j = 1; j <= 14; ++j) {
    p._M[j - 1] = W[j] * L[j];
  }
  _Base::_P[0] = p;

  // Constraints: device sizes
  // Wmin <= W <= Wmax; Lmin <= L <= Lmax;
  for (j = 1; j <= 14; ++j) {
    _Base::_P.push_back(posynomial<_Tp>(W[j] / Wmax));
    _Base::_P.push_back(posynomial<_Tp>(Wmin / W[j]));
    _Base::_P.push_back(posynomial<_Tp>(L[j] / Lmax));
    _Base::_P.push_back(posynomial<_Tp>(Lmin / L[j]));
  }

  // Constraints: over drive voltages
  // VODmin <= VOD <= VODmax; 0.1 <= VOD[12] <= VODmax;
  for (j = 1; j <= 13; ++j) {
    _Base::_P.push_back(posynomial<_Tp>(VOD[j] / VODmax));
    _Base::_P.push_back(posynomial<_Tp>(VODmin / VOD[j]));
  }
  // xxx _Base::_P.push_back( posynomial<_Tp>( VOD[12] / VODmax  ));
  // xxx _Base::_P.push_back( posynomial<_Tp>( 0.1000  / VOD[12] ));
  // xxx _Base::_P.push_back( posynomial<_Tp>( VOD[13] / VODmax  ));
  // xxx _Base::_P.push_back( posynomial<_Tp>( VODmin  / VOD[13] ));

  // Constraints: input common voltage
  _Base::_P.push_back(posynomial<_Tp>(VOD[1] / (VCMmin - Vss - VTP - VTN)));
  p = posynomial<_Tp>(NUM_VARS, 2);
  p._M[0] = VOD[1] / (Vdd - VCMmax + VTP);
  p._M[1] = VOD[5] / (Vdd - VCMmax + VTP);
  _Base::_P.push_back(p);

  // Constraints: output voltage range
  _Base::_P.push_back(posynomial<_Tp>(VOD[6] / (VOUTmin - Vss)));
  _Base::_P.push_back(posynomial<_Tp>(VOD[7] / (Vdd - VOUTmax)));

  // Constraint: Quiescent Power
  // (I5+I7+I8+I9)*(Vdd-Vss) <= POWERmax
  p = posynomial<_Tp>(NUM_VARS, 4);
  p._M[0] = I[5] / (Vdd - Vss) / POWERmax;
  p._M[1] = I[7] / (Vdd - Vss) / POWERmax;
  p._M[2] = I[8] / (Vdd - Vss) / POWERmax;
  p._M[3] = I[9] / (Vdd - Vss) / POWERmax;
  _Base::_P.push_back(p);

  // Constraint: Open Loop Gain
  // 10^(OLGmin/20) <= OLG;
  monomial<_Tp> OLG = sqrt(KP * KN * W[1] / L[1] * W[6] / L[6] / I[1] / I[6]) *
                      2.0 / (LAMBDAN + LAMBDAP) / (LAMBDAN + LAMBDAP);
  _Base::_P.push_back(posynomial<_Tp>(pow(10, OLGmin / 20) / OLG));

  // Constraint: Unit Gain Bandwith
  // UGBmin <= UGB;
  monomial<_Tp> WUGB = Gm[1] / Cc;
  monomial<_Tp> UGB = WUGB / 2.0 / PI;
  _Base::_P.push_back(posynomial<_Tp>(UGBmin / UGB));

  // Constraints: four poles
  posynomial<_Tp> C1 = Cgs[6] + Cgd[2] + Cgd[4];
  posynomial<_Tp> C2 = Cgs[3] + Cgs[4] + Cgd[1];
  posynomial<_Tp> C3 = posynomial<_Tp>(Cgd[6]) + Cgd[7];
  monomial<_Tp> WP1_inv = Cc * OLG / Gm[1];
  posynomial<_Tp> WP2_inv = (C1 * C3) / Gm[6] / Cc + Cload / Gm[6] +
                            (C1 + C3) / Gm[6] + C1 / Gm[6] * Cload / Cc;
  posynomial<_Tp> WP3_inv = C2 / Gm[3];
  posynomial<_Tp> WP4_inv = C1 / Gm[6] * Fc;

  // 2,3,4-pole more than 10 times of 1-pole, eqn(32)
  _Base::_P.push_back(posynomial<_Tp>(WP2_inv / WP1_inv * 10.0));
  _Base::_P.push_back(posynomial<_Tp>(WP3_inv / WP1_inv * 10.0));
  _Base::_P.push_back(posynomial<_Tp>(WP4_inv / WP1_inv * 10.0));

  // 2,3,4-pole more than 2.5 times of unitity gain bandwidth
  _Base::_P.push_back(posynomial<_Tp>(WP2_inv * WUGB * 2.5));
  _Base::_P.push_back(posynomial<_Tp>(WP3_inv * WUGB * 2.5));
  _Base::_P.push_back(posynomial<_Tp>(WP4_inv * WUGB * 2.5));

  // Constraint: Phase Margin
  _Base::_P.push_back(
      posynomial<_Tp>((WP3_inv + WP4_inv) * WUGB / PI / (0.5 - PMmin / 180.0)));

  // Constraint: CMRR
  // 10^(CMRRmin/20) <= CMRR;
  monomial<_Tp> CMRR = sqrt(KP * KN * W[1] / L[1] * W[3] / L[3] / I[1] / I[3]) /
                       LAMBDAP / (LAMBDAN + LAMBDAP);
  _Base::_P.push_back(posynomial<_Tp>(pow(10, CMRRmin / 20) / CMRR));

  // Constraint: NPSRR
  // 10^(NPSRRmin/20) <= NPSRR;
  monomial<_Tp> NPSRR =
      sqrt(KP * KN * W[1] / L[1] * W[6] / L[6] / I[1] / I[6]) / LAMBDAN /
      (LAMBDAN + LAMBDAP);
  _Base::_P.push_back(posynomial<_Tp>(pow(10, NPSRRmin / 20) / NPSRR));

  // Constraints: Slew Rate
  _Base::_P.push_back(posynomial<_Tp>(Cc / I[5] * SRmin));
  _Base::_P.push_back(
      posynomial<_Tp>(C3 / I[7] * SRmin + (Cload + Cc) / I[7] * SRmin));

  // Constraint: IRSN
  // IRSN <= IRSNmax^2;
  posynomial<_Tp> IRSN =
      posynomial<_Tp>(2.0 / 1000.0 * KFP / COX / W[1] / L[1]) +
      2.0 / 1000.0 * KFN / COX * UN / UP * L[1] / W[1] / L[3] / L[3];
  _Base::_P.push_back(posynomial<_Tp>(IRSN / IRSNmax / IRSNmax));

  // check the parameter values
  cout << "constraints = " << _Base::_P.size() - 1 << "\n"
       << "Ibias = " << scientific << showpoint << setprecision(3)
       << exp(Ibias(x)) << "\n"
       << "Acof  = " << scientific << showpoint << setprecision(3) << exp(A(x))
       << "\n"
       << "Bcof  = " << scientific << showpoint << setprecision(3) << exp(B(x))
       << "\n"
       << "Cload = " << scientific << showpoint << setprecision(3) << Cload
       << "\n"
       << "Cc    = " << scientific << showpoint << setprecision(3) << Cc
       << "\n";
  for (j = 1; j <= 14; ++j) {
    cout << "L[" << setw(2) << j << "] = " << scientific << showpoint
         << setprecision(3) << exp(L[j](x)) << "\t"
         << "W[" << setw(2) << j << "] = " << scientific << showpoint
         << setprecision(3) << exp(W[j](x)) << "\n";
  }
  cout << endl;
}

template <typename _Tp> ex_opamp4<_Tp>::~ex_opamp4() {}

// Explicit instantiation
#include <aa.h>
// xxx template class ex_opamp4<double>;
template class ex_opamp4<AAF>;
// xxx template ex_opamp4<double>::ex_opamp4(const std::valarray<double>& x);
template ex_opamp4<AAF>::ex_opamp4(const std::valarray<double> &x);
template ex_opamp4<AAF>::ex_opamp4(const std::valarray<AAF> &x);
