#include "ex_opamp2.hpp"
#include "posynomial.hpp"
#include <aa.h>
#include <cassert>
#include <cmath>
#include <valarray>
#include <vector>

using namespace std;

// Example of two-stage op-amp.

static const double Pi = 3.141592654,
                    KT = 1.38e-23 * 300 // ???
    ;

// HSPICE Level 1 parameters for the technology
static const AAF TOX = 20e-9,        // Oxide thickness (m)
    LD = interval(0.19e-6, 0.21e-6), // Lateral diffusion (m)
    // LD = 0.2e-6,        // Lateral diffusion (m)
    COX = 3.45e-11 / TOX, // oxide capacitance per unit area (F/m^2)

    VTN = interval(0.65, 0.75), // NMOS threshold voltage (V)
    // VTN = 0.7,          // NMOS threshold voltage (V)
    LAMBDAN = 0.03, // NMOS channel length modulation (V^-1)
    UN = 600e-4,    // NMOS carrier mobility (m^2/(Vs))
    // KN = 103.59e-6,     // NMOS intrinsic transconductance parameter (A/V^2)
    KN = UN * COX,
                 // GAMMAN = 0.298,     // NMOS body effect factor (V^0.5)
    // PHIN = 0.688,       // NMOS surface inversion potential
    // RSHN = 40,          // NMOS diffusion sheet resistance (ohm per unit
    // area)
    CJN = 271e-6,   // NMOS zero-bias bulk junction capacitace (F/m^2)
    CJSWN = 600e-6, // NMOS zero-bias sidewall bulk junction capacitance (F/m)
    // MJN = 0.5,          // NMOS bulk junction grading coefficient (1)
    // PBN = 0.904,        // NMOS bulk junction contact potential (V)
    // CGDON = 346e-6,     // NMOS gate-grain overlap capacitance (F/m)
    // CGSON = 346e-6,     // NMOS gate-source overlap capacitance (F/m)
    KFN = 4e-24, // NMOS flicker noise coefficient

    VTP = interval(-0.95, -0.85), // PMOS threshold voltage (V)
    // VTP = -0.9,         // PMOS threshold voltage (V)
    LAMBDAP = 0.06, // PMOS channel length modulation (V^-1)
    UP = 200e-4,    // PMOS carrier mobility (m^2/(Vs))
    // KP = 34.53e-6,      // PMOS intrinsic transconductance parameter (A/V^2)
    KP = UP * COX,
                 // GAMMAP = 0.471,     // PMOS body effect factor (V^0.5)
    // PHIP = 0.730,       // PMOS surface inversion potential
    // RSHP = 60,          // PMOS diffusion sheet resistance (ohm per unit
    // area)
    CJP = 423e-6,   // PMOS zero-bias bulk junction capacitace (F/m^2)
    CJSWP = 1.2e-9, // PMOS zero-bias sidewall bulk junction capacitance (F/m)
    // MJP = 0.5,          // PMOS bulk junction grading coefficient (1)
    // PBP = 0.928,        // PMOS bulk junction contact potential (V)
    // CGDOP = 346e-6,     // PMOS gate-grain overlap capacitance (F/m)
    // CGSOP = 346e-6,     // PMOS gate-source overlap capacitance (F/m)
    KFP = 2e-24; // PMOS flicker noise coefficient

// Design Parameters
static const AAF Vdd = interval(4.9, 5.1), // positive supply voltage (V)
    // Vdd = 5.0,          // positive supply voltage (V)
    Vss = 0.,   // negative supply voltage (V)
    CL = 3e-12; // loading capacitance (F)

// Specifications
static const double Lmax = 10e-6, // max device length (m)
    Lmin = 0.8e-6,                // min device length (m)
    Wmax = 1000e-6,               // max device width  (m)
    Wmin = 2e-6,                  // min device width  (m)
    Amax = 10000e-12,             // maximum area (m^2)

    Vcmmax = 0.5 * 5,  // max common-mode input voltage (V)
    Vcmmin = 0.5 * 5,  // min common-mode input voltage (V)
    Voutmax = 0.9 * 5, // max output voltage (V)
    Voutmin = 0.1 * 5, // min output voltage (V)
    Pmax = 5e-3,       // max quiescent power (W)
    Vodmin = 130e-3,   // min gate overdrive (V)
    deltaVo = 200e-3,  // typical overdrive voltage (V)

    Gmin = 80,      // min open-loop gain (dB)
    Umin = 70e6,    // min unit gain bandwidth (Hz)
    PMmin = Pi / 3, // min phase margin (radius)
    SRmin = 10e6,   // min slew rate (V/s)
    CMRRmin = 60,   // min common-mode rejection ratio (dB)
    NPSRRmin = 80,  // min neg. power supply rejection ratio (dB)
    PPSRRmin = 80,  // min pos. power supply rejection ratio (dB)
    Irsn = 300e-9,  // Input-referred spot noise, 1kHz (nV/Hz^0.5)

    alpha0 = 1000e-12, // ???
    alpha1 = 100.,     // ???
    alpha2 = 8.;       // ???

// Transconductance (NMOS), Eq.(51)
// ignore channel-length modulation
template <typename _Tp>
inline monomial<_Tp> gm_NMOS(size_t W, size_t L, const monomial<_Tp> &ID) {
  monomial<_Tp> temp(NUM_VARS);
  temp._a[W] = 1;
  temp._a[L] = -1;
  return sqrt(2. * UN * COX * temp * ID);
}

// transconductance (PMOS), Eq.(51)
// ignore channel-length modulation
template <typename _Tp>
inline monomial<_Tp> gm_PMOS(size_t W, size_t L, const monomial<_Tp> &ID) {
  monomial<_Tp> temp(NUM_VARS);
  temp._a[W] = 1;
  temp._a[L] = -1;
  return sqrt(2. * UP * COX * temp * ID);
}

// Problem setup
template <typename _Tp>
template <typename _Up>
ex_opamp2<_Tp>::ex_opamp2(const std::valarray<_Up> &x) : _Base() {
  _Base::_P.push_back(posynomial<_Tp>(NUM_VARS, 1)); // preserve for obj. fct.
  posynomial<_Tp> p(NUM_VARS, 1);

  std::cout << "  \\begin{tabular}{|l|c|l|}\n"
            << "    \\hline\n"
            << "    Performance               & Units      & Value \\\\\n"
            << "    \\hline\\hline\n"
            << "    \\hline\n";

  monomial<_Tp> mL1(NUM_VARS);
  mL1._a[L1] = 1;
  monomial<_Tp> mL3(NUM_VARS);
  mL3._a[L3] = 1;
  monomial<_Tp> mL5(NUM_VARS);
  mL5._a[L5] = 1;
  monomial<_Tp> mL6(NUM_VARS);
  mL6._a[L6] = 1;
  monomial<_Tp> mW1(NUM_VARS);
  mW1._a[W1] = 1;
  monomial<_Tp> mW3(NUM_VARS);
  mW3._a[W3] = 1;
  monomial<_Tp> mW5(NUM_VARS);
  mW5._a[W5] = 1;
  monomial<_Tp> mW6(NUM_VARS);
  mW6._a[W6] = 1;
  monomial<_Tp> mW7(NUM_VARS);
  mW7._a[W7] = 1;
  monomial<_Tp> mW8(NUM_VARS);
  mW8._a[W8] = 1;
  monomial<_Tp> mCc(NUM_VARS);
  mCc._a[Cc] = 1;
  monomial<_Tp> mIbias(NUM_VARS);
  mIbias._a[Ibias] = 1;

  monomial<_Tp> mT2(NUM_VARS);
  mT2._a[T2] = 1;
  monomial<_Tp> mT3(NUM_VARS);
  mT3._a[T3] = 1;
  monomial<_Tp> mT4(NUM_VARS);
  mT4._a[T4] = 1;

  // III.B. Limits on Device Sizes, Eq.(7) (Mono.)
  _Base::_P.push_back(posynomial<_Tp>(mL1 / Lmax));
  _Base::_P.push_back(posynomial<_Tp>(_Tp(Lmin) / mL1));
  _Base::_P.push_back(posynomial<_Tp>(mL3 / Lmax));
  _Base::_P.push_back(posynomial<_Tp>(Lmin / mL3));
  _Base::_P.push_back(posynomial<_Tp>(mL5 / Lmax));
  _Base::_P.push_back(posynomial<_Tp>(Lmin / mL5));
  _Base::_P.push_back(posynomial<_Tp>(mL6 / Lmax));
  _Base::_P.push_back(posynomial<_Tp>(Lmin / mL6));
  _Base::_P.push_back(posynomial<_Tp>(mW1 / Wmax));
  _Base::_P.push_back(posynomial<_Tp>(Wmin / mW1));
  _Base::_P.push_back(posynomial<_Tp>(mW3 / Wmax));
  _Base::_P.push_back(posynomial<_Tp>(Wmin / mW3));
  _Base::_P.push_back(posynomial<_Tp>(mW5 / Wmax));
  _Base::_P.push_back(posynomial<_Tp>(Wmin / mW5));
  _Base::_P.push_back(posynomial<_Tp>(mW6 / Wmax));
  _Base::_P.push_back(posynomial<_Tp>(Wmin / mW6));
  _Base::_P.push_back(posynomial<_Tp>(mW7 / Wmax));
  _Base::_P.push_back(posynomial<_Tp>(Wmin / mW7));
  _Base::_P.push_back(posynomial<_Tp>(mW8 / Wmax));
  _Base::_P.push_back(posynomial<_Tp>(Wmin / mW8));

  // III.C. Area, Eq.(8) (Posy.)
  // p = posynomial<_Tp>(NUM_VARS, 8);
  // p._M[0]._b = log(alpha0/Amax);
  // p._M[1]._a[Cc] =  1.;
  // p._M[1]._b =  log(alpha1/Amax);
  // p._M[2]._a[W1] = p._M[2]._a[L1] = 1;
  // p._M[2]._b = log(2.*alpha2/Amax);
  // p._M[3]._a[W3] = p._M[3]._a[L3] = 1;
  // p._M[3]._b = log(2.*alpha2/Amax);
  // p._M[4]._a[W5] = p._M[4]._a[L5] = 1;
  // p._M[4]._b = log(alpha2/Amax);
  // p._M[5]._a[W6] = p._M[5]._a[L6] = 1;
  // p._M[5]._b = log(alpha2/Amax);
  // p._M[6]._a[W7] = p._M[6]._a[L7] = 1;
  // p._M[6]._b = log(alpha2/Amax);
  // p._M[7]._a[W8] = p._M[7]._a[L8] = 1;
  // p._M[7]._b = log(alpha2/Amax);
  // _Base::_P.push_back(p);

  // III.D. Systematic Input Offset Voltage
  // Eq.(9) equality constraints handled by two inequality constraints
  monomial<_Tp> M22(NUM_VARS);
  M22._a[W3] = M22._a[L6] = M22._a[W7] = 1;
  M22._a[L3] = M22._a[W6] = M22._a[W5] = -1;

  _Base::_P.push_back(posynomial<_Tp>(2. / 1.01 * M22));
  _Base::_P.push_back(posynomial<_Tp>(0.99 / 2. / M22));

  // IV BIAS, CONDITIONS, SIGNAL SWING, AND POWER CONSTRAINTS
  monomial<_Tp> I8(NUM_VARS);
  I8._a[Ibias] = 1;

  // Eq.(10): Note L5==L8
  monomial<_Tp> I5(NUM_VARS);
  I5._a[W5] = I5._a[Ibias] = 1;
  I5._a[W8] = -1;

  // Eq.(10): Note L7==L8
  monomial<_Tp> I7(NUM_VARS);
  I7._a[W7] = I7._a[Ibias] = 1;
  I7._a[W8] = -1;

  // Eq.(11)
  monomial<_Tp> I1 = I5 / 2.;

  // IV.A  Bias Conditions
  // Common mode input range, Eq.(12) (Posy.)
  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> T12(NUM_VARS);
  T12._a[L3] = 1.;
  T12._a[W3] = -1.;
  p._M[0] = sqrt(T12 * I1 / (UN * COX / 2.)) / (Vcmmin - Vss - VTP - VTN);
  _Base::_P.push_back(p);

  // Common-mode input range, Eq.(13) (Posy.)
  p = posynomial<_Tp>(NUM_VARS, 2);
  monomial<_Tp> T13a(NUM_VARS);
  T13a._a[L1] = 1.;
  T13a._a[W1] = -1.;
  p._M[0] = sqrt(T13a * I1 / (UP * COX / 2.)) / (Vdd - Vcmmax + VTP);
  monomial<_Tp> T13b(NUM_VARS);
  T13b._a[L5] = 1.;
  T13b._a[W5] = -1.;
  p._M[1] = sqrt(T13b * I5 / (UP * COX / 2.)) / (Vdd - Vcmmax + VTP);
  _Base::_P.push_back(p);

  // Output voltage swing, Eq.(14) (Posy.???)
  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> T14(NUM_VARS);
  T14._a[L6] = 1.;
  T14._a[W6] = -1.;
  p._M[0] = sqrt(T14 * I7 / (UN * COX / 2.)) / (Voutmin - Vss);
  _Base::_P.push_back(p);

  // Check
  std::cout << "    Output Range(min)         & V          & $"
            << bound(exp(p(x))) << "V_{DD}$ \\\\\n";

  // Output voltage swing, Eq.(15) (Posy.???)
  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> T15(NUM_VARS);
  T15._a[L7] = 1.;
  T15._a[W7] = -1.;
  p._M[0] = sqrt(T15 * I7 / (UP * COX / 2.)) / (Vdd - Voutmax);
  _Base::_P.push_back(p);
  // Check
  std::cout << "    Output Range(max)         & V          & $"
            << bound(exp(p(x))) << "V_{DD}$ \\\\\n";

  // IV.B Gate Overdrive, Eq.(16) (Mono.)
  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> T161(NUM_VARS);
  T161._a[L1] = -1.;
  T161._a[W1] = 1.; // M1
  T161 = sqrt(T161 * (UP * COX / 2.0) / I1);
  p._M[0] = T161 * (Vodmin);
  _Base::_P.push_back(p);

  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> T163(NUM_VARS);
  T163._a[L3] = -1.;
  T163._a[W3] = 1.; // M3
  T163 = sqrt(T163 * (UN * COX / 2.0) / I1);
  p._M[0] = T163 * Vodmin;
  _Base::_P.push_back(p);

  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> T165(NUM_VARS);
  T165._a[L5] = -1.;
  T165._a[W5] = 1.; // M5
  T165 = sqrt(T165 * (UP * COX / 2.0) / I5);
  p._M[0] = T165 * Vodmin;
  _Base::_P.push_back(p);

  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> T166(NUM_VARS);
  T166._a[L6] = -1.;
  T166._a[W6] = 1.; // M6
  T166 = sqrt(T166 * (UN * COX / 2.0) / I7);
  p._M[0] = T166 * Vodmin;
  _Base::_P.push_back(p);

  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> T167(NUM_VARS);
  T167._a[L7] = -1.;
  T167._a[W7] = 1.; // M7
  T167 = sqrt(T167 * (UP * COX / 2.0) / I7);
  p._M[0] = T167 * Vodmin;
  _Base::_P.push_back(p);

  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> T168(NUM_VARS);
  T168._a[L8] = -1.;
  T168._a[W8] = 1.; // M8
  T168 = sqrt(T168 * (UP * COX / 2.0) / I8);
  p._M[0] = T168 * Vodmin;
  _Base::_P.push_back(p);

  // IV.C  Quiescent Power, Eq.(17) (Posy.)
  posynomial<_Tp> qpower(NUM_VARS, 3);
  qpower._M[0]._a[Ibias] = 1.;
  qpower._M[1] = I5;
  qpower._M[2] = I7;
  qpower *= (Vdd - Vss);
  _Base::_P.push_back(posynomial<_Tp>(qpower / _Tp(Pmax)));
  // Check
  std::cout << "    Power                     & mW         & $"
            << bound(exp(qpower(x)) * 1e3) << "$ \\\\\n";

  // V. Small-signal transfer function constraints
  // V.A. Small-signal transfer function
  // Open-loop voltage gain, Eq.(20) (Mono.)
  monomial<_Tp> Av(NUM_VARS);
  Av._a[W2] = Av._a[W6] = 1.;
  Av._a[L2] = Av._a[L6] = -1.;
  Av = sqrt(Av / I1 / I7 * (UN * UP));
  Av *= 2. * COX / (LAMBDAN + LAMBDAP) / (LAMBDAN + LAMBDAP);

  // Open-loop voltage gain, Eq.(20) (Mono.)
  p = posynomial<_Tp>(NUM_VARS, 1);
  p._M[0]._b = (Gmin / 20) * log(10.);
  p._M[0] /= Av;
  _Base::_P.push_back(p);
  // Check
  std::cout << "    Gain                      & dB         & $"
            << bound(Av(x) / log(10.) * 20) << "$ \\\\\n";

  monomial<_Tp> gm1(gm_PMOS(W1, L1, I1));
  monomial<_Tp> gm2(gm_PMOS(W2, L2, I1));
  monomial<_Tp> gm3(gm_NMOS(W3, L3, I1));
  monomial<_Tp> gm6(gm_NMOS(W6, L6, I7));

  // output conductance Eq(52)
  monomial<_Tp> go2 = I1 * LAMBDAP;
  monomial<_Tp> go4 = I1 * LAMBDAN;
  monomial<_Tp> go5 = I5 * LAMBDAP;
  monomial<_Tp> go6 = I7 * LAMBDAN;
  monomial<_Tp> go7 = I7 * LAMBDAP;

  posynomial<_Tp> Cgd1(NUM_VARS, 1); // gate-to-drain
  Cgd1._M[0]._a[W1] = 1;
  Cgd1._M[0]._b = log(LD * COX); // W*LD*Cox

  posynomial<_Tp> Cgd2(NUM_VARS, 1);
  Cgd2._M[0]._a[W2] = 1;
  Cgd2._M[0]._b = log(LD * COX);

  posynomial<_Tp> Cgd4(NUM_VARS, 1);
  Cgd4._M[0]._a[W4] = 1;
  Cgd4._M[0]._b = log(LD * COX);

  posynomial<_Tp> Cgd6(NUM_VARS, 1);
  Cgd6._M[0]._a[W6] = 1;
  Cgd6._M[0]._b = log(LD * COX);

  posynomial<_Tp> Cgd7(NUM_VARS, 1);
  Cgd7._M[0]._a[W7] = 1;
  Cgd7._M[0]._b = log(LD * COX);

  posynomial<_Tp> Cgs3(NUM_VARS, 2); // gate-to-source
  Cgs3._M[0]._a[W3] = 1;
  Cgs3._M[0]._a[L3] = 1;
  Cgs3._M[0]._b = log(2. / 3. * COX); // (2/3)*W*L*Cox
  Cgs3._M[1]._a[W3] = 1;
  Cgs3._M[1]._b = log(LD * COX); // + W*LD*Cox

  posynomial<_Tp> Cgs4(NUM_VARS, 2);
  Cgs4._M[0]._a[W4] = 1;
  Cgs4._M[0]._a[L4] = 1;
  Cgs4._M[0]._b = log(2. / 3. * COX);
  Cgs4._M[1]._a[W4] = 1;
  Cgs4._M[1]._b = log(LD * COX);

  posynomial<_Tp> Cgs6(NUM_VARS, 2);
  Cgs6._M[0]._a[W6] = 1;
  Cgs6._M[0]._a[L6] = 1;
  Cgs6._M[0]._b = log(2. / 3. * COX);
  Cgs6._M[1]._a[W6] = 1;
  Cgs6._M[1]._b = log(LD * COX);

  posynomial<_Tp> Csb0(NUM_VARS, 2);
  Cgs6._M[0]._a[W6] = 1;
  Cgs6._M[0]._a[L6] = 1;
  Cgs6._M[0]._b = log(2. / 3. * COX);
  Cgs6._M[1]._a[W6] = 1;
  Cgs6._M[1]._b = log(LD * COX);

  // xxx posynomial<_Tp> Cdb1(NUM_VARS, 1); // drain-to-bulk
  // xxx posynomial<_Tp> Cdb2(NUM_VARS, 1); // not implemented yet
  // xxx posynomial<_Tp> Cdb3(NUM_VARS, 1); // not implemented yet
  // xxx posynomial<_Tp> Cdb4(NUM_VARS, 1); // not implemented yet
  // xxx posynomial<_Tp> Cdb6(NUM_VARS, 1); // not implemented yet
  // xxx posynomial<_Tp> Cdb7(NUM_VARS, 1); // not implemented yet

  // The dominant pole: Eq.(21)
  monomial<_Tp> p1(NUM_VARS);
  p1._a[Cc] = -1;
  p1 *= gm1 / Av;

  monomial<_Tp> MCc(NUM_VARS);
  MCc._a[Cc] = 1.;

  // Capacitance at the gate of M6, Eq.(23)
  // posynomial<_Tp> C1 = Cgs6 + Cdb2 + Cdb4 + Cgd2 + Cgd4;
  posynomial<_Tp> C1 = Cgs6 + Cgd2 + Cgd4; // !! omit Cdb2, Cdb4

  // Total capacitance at the output node, Eq(24)
  posynomial<_Tp> CTL(NUM_VARS, 1);
  CTL._M[0]._b = log(CL);
  // CTL += Cdb6 + Cdb7 + Cgd6 + Cgd7; // Eq.(24)
  CTL += Cgd6 + Cgd7; // !! omit Cdb6, Cdb7

  // Inverse of output pole, Eq.(22)
  posynomial<_Tp> inv_p2 = (C1 * MCc + C1 * CTL + CTL * MCc) / (gm6 * MCc);

  // Capacitance at the gate of M3, Eq(26)
  // posynomial<_Tp> C2 = Cgs3 + Cgs4 + Cdb1 + Cdb3 + Cgd1;
  posynomial<_Tp> C2 = Cgs3 + Cgs4 + Cgd1; // omit Cdb1 Cdb3

  // Inverse of the mirror pole, Eq.(25)
  posynomial<_Tp> inv_p3 = C2 / gm3;

  // Inverse of the conpensation pole, Eq.(27)
  posynomial<_Tp> inv_p4 = C1 / gm6;

  // 3dB bandwidth Eq.(31) (Posy) ???
  // p = posynomial<_Tp>(NUM_VARS, 1);
  // p._M[0]._b = log(10e6*2.*Pi);
  // p._M[0] /= p1;
  // _Base::_P.push_back(p);

  // Dominant pole conditions Eq.(32) (Posy)
  monomial<_Tp> Tlog10(NUM_VARS);
  Tlog10._b = log(10.);
  posynomial<_Tp> T321 = inv_p2 * p1 * Tlog10;
  _Base::_P.push_back(posynomial<_Tp>(T321)); // p1/p2 < 0.1

  posynomial<_Tp> T322 = inv_p3 * p1 * Tlog10;
  _Base::_P.push_back(posynomial<_Tp>(T322)); // p1/p3 < 0.1

  posynomial<_Tp> T323 = inv_p4 * p1 * Tlog10;
  _Base::_P.push_back(posynomial<_Tp>(T323)); // p1/p4 < 0.1
                                              // xxx std::cout << "pole = "
  // xxx 	    << bound(exp(p1(x))) << ", " << bound(1./exp(inv_p2(x))) << ",
  // "
  // xxx           << bound(1./exp(inv_p3(x))) << ", " <<
  // bound(1./exp(inv_p4(x)))
  // xxx 	    << std::endl;

  // Unity-gain bandwidth, Eq.(34) (Mono)
  monomial<_Tp> omegac(NUM_VARS);
  omegac._a[Cc] = -1;
  omegac *= gm1;
  p = posynomial<_Tp>(NUM_VARS, 1);
  p._M[0]._b = log(Umin);
  p._M[0] /= omegac;
  _Base::_P.push_back(p);
  // Check
  std::cout << "    Unity Gain Freq.          & MHz        & $"
            << bound(exp(omegac(x)) / 2. / Pi * 1e-6) << "$ \\\\\n";

  // Phase margin, Eq.(36) (Posy)
  monomial<_Tp> mTT2(NUM_VARS);
  mTT2._a[T2] = 0.7;
  monomial<_Tp> mTT3(NUM_VARS);
  mTT3._a[T3] = 0.7;
  monomial<_Tp> mTT4(NUM_VARS);
  mTT4._a[T4] = 0.7;

  posynomial<_Tp> phase = (posynomial<_Tp>(mTT2) + mTT3 + mTT4) * 0.75;
  _Base::_P.push_back(posynomial<_Tp>(phase / (Pi / 2. - PMmin)));

  _Base::_P.push_back(inv_p2 * omegac / mT2);
  _Base::_P.push_back(inv_p3 * omegac / mT3);
  _Base::_P.push_back(inv_p4 * omegac / mT4);

  // Check
  std::cout << "    Phase Margin              & degree     & $"
            << bound(90 - exp(phase(x)) * (180. / Pi)) << "$ \\\\\n";
  // @todo: use more accurate equation for checking

  // Slew rate, Eq.(37) (Posy)
  p = posynomial<_Tp>(NUM_VARS, 1);
  p._M[0] = MCc / I1 * (SRmin / 2.);
  _Base::_P.push_back(p);
  // Check
  std::cout << "    Slew Rate 1               & V/$\\mu$s   & $"
            << bound(SRmin / exp(p(x)) * 1e-6) << "$ \\\\\n";

  posynomial<_Tp> T37 = (CTL + MCc) / (I7 / (SRmin));
  _Base::_P.push_back(posynomial<_Tp>(T37));
  // Check
  std::cout << "    Slew Rate 2               & V/$\\mu$s   & $"
            << bound(SRmin / exp(T37(x)) * 1e-6) << "$ \\\\\n";

  // CMRR, Eq.(38) (Mono)
  p = posynomial<_Tp>(NUM_VARS, 1);
  monomial<_Tp> CMRR(NUM_VARS);
  CMRR._a[W1] = CMRR._a[W3] = 1.;
  CMRR._a[L1] = CMRR._a[L3] = -1.;
  CMRR = sqrt(CMRR / I5 / I5 * (UN * UP)) *
         (2. * COX / (LAMBDAN + LAMBDAP) / LAMBDAP);
  p._M[0]._b = CMRRmin / 20. * log(10.);
  p._M[0] /= CMRR;
  _Base::_P.push_back(p);
  // Check
  std::cout << "    CMRR                      & dB         & $"
            << bound(CMRR(x) / log(10.) * 20.) << "$ \\\\\n";

  // Neg. PSRR, Eq.(40,41) (Inv. Posy) ???
  posynomial<_Tp> inv_Ap = (posynomial<_Tp>(go2) + go4) * go6 / gm2 / gm6;
  // @todo: add Eq.(41) constraints
  // Check
  std::cout << "    Neg. PSRR at low-freq     & dB         & $"
            << bound(-inv_Ap(x) / log(10.) * 20.) << "$ \\\\\n";

  // Pos. PSRR, Eq.(42) (Neither)
  // Check
  // _Tp pPSRR =
  //   (log(2.) + (gm2*gm3*gm6)(x) - log(exp(go2(x))+ exp(go2(x)))
  //    - log (2.*exp((gm3*go7)(x)) - exp((gm6*go5)(x))))/log(10.)*20.;
  // xxx std::cout << "pos. PSRR (db) = " << bound(pPSRR) <<std::endl;

  // Input-referred spot noise, Eq.(43) (Posy) ???
  monomial<_Tp> alpha43a(NUM_VARS);
  alpha43a._a[W1] = alpha43a._a[L1] = -1;
  alpha43a._b = log(2. * KFP / COX);

  monomial<_Tp> alpha43b(NUM_VARS);
  alpha43b._a[L1] = 2;
  alpha43b._a[L3] = -2;
  alpha43b._b = log(KFN / KFP * UN / UP);

  posynomial<_Tp> alpha43(NUM_VARS, 2);
  alpha43._M[1] = alpha43b;
  alpha43 *= alpha43a;

  monomial<_Tp> beta43a(NUM_VARS);
  beta43a._a[W1] = 1;
  beta43a._a[L1] = -1;
  beta43a._b = log(2. * UP / COX);
  beta43a = sqrt(beta43a) * (3. / 16. / KT);

  monomial<_Tp> beta43b(NUM_VARS);
  beta43b._a[W3] = 1;
  beta43b._a[L3] = -1;
  beta43b._a[L1] = 1;
  beta43b._a[W1] = -1;
  beta43b._b = log(UN / UP);
  beta43b = beta43b;

  posynomial<_Tp> beta43(NUM_VARS, 2);
  beta43._M[1] = beta43b;
  beta43 /= beta43a;

  double f = 1000.;
  posynomial<_Tp> Sin2 = alpha43 / f + beta43;
  // Check
  _Base::_P.push_back(posynomial<_Tp>(Sin2 / (Irsn * Irsn)));
  std::cout << "    Noise, Flicker            & nV$/\\sqrt{\\mathrm{Hz}}$ & $"
            << bound(sqrt(exp(Sin2(x))) * 1e9) << "$ \\\\\n";

  // Input-referred total noise, Eq.(44) (Posy)
  // @todo: add Eq.(44)

  // Objective: sum of area
  // area = 2*W1*L1 + 2*W3*L3 + W5*L5 + W6*L6 + W7*L7 + W8*L8
  //_Base::_P[0] = posynomial<_Tp>(NUM_VARS, 6);
  //_Base::_P[0]._M[0]._a[W1] = _Base::_P[0]._M[0]._a[L1] = 1;
  //_Base::_P[0]._M[0]._b = log(2.);
  //_Base::_P[0]._M[1]._a[W3] = _Base::_P[0]._M[1]._a[L3] = 1;
  //_Base::_P[0]._M[1]._b = log(2.);
  //_Base::_P[0]._M[2]._a[W5] = _Base::_P[0]._M[2]._a[L5] = 1;
  //_Base::_P[0]._M[3]._a[W6] = _Base::_P[0]._M[3]._a[L6] = 1;
  //_Base::_P[0]._M[4]._a[W7] = _Base::_P[0]._M[4]._a[L7] = 1;
  //_Base::_P[0]._M[5]._a[W8] = _Base::_P[0]._M[5]._a[L8] = 1;

  // Maximize unity-gain bandwidth
  _Base::_P[0]._M[0] /= omegac;

  std::cout << "    \\hline\n"
            << "  \\end{tabular}\n"
            << std::endl;
}

template <typename _Tp> ex_opamp2<_Tp>::~ex_opamp2() {}

// Explicit instantiation
#include <aa.h>
// template class ex_opamp2<double>;
template class ex_opamp2<AAF>;
// template ex_opamp2<double>::ex_opamp2(const std::valarray<double>& x);
template ex_opamp2<AAF>::ex_opamp2(const std::valarray<double> &x);
template ex_opamp2<AAF>::ex_opamp2(const std::valarray<AAF> &x);
