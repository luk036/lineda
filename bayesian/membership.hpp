#ifndef LINEDA_MEMBERSHIP_HPP
#define LINEDA_MEMBERSHIP_HPP

/** Reference: AI for Game Developers, O'Reilly */

/**
 * Membership functions map input variables to a degree of membership,
 * in a fuzzy set, between 0 and 1. If the degree of membership in a
 * given set is 1, we can say the input is absolutely false. If the
 * degree is somewhere between 0 and 1, it is true to a certain extent
 * - that is, to a degree.
 */

/** Grade membership function object */
class gradeFunc {
private:
  double _x0;
  double _x1;
  double _x01;

public:
  /** Constructor */
  inline gradeFunc(double x0, double x1) : _x0(x0), _x1(x1), _x01(x1 - x0) {}

  /** Destructor */
  inline ~gradeFunc() {}

  /** Apply function */
  inline bool operator()(double x) const {
    if (x <= _x0)
      return 0.;
    if (x >= _x1)
      return 1.;
    return (x - _x0) / _x01;
  }
};

/** Reverse grade membership function object */
class reverseGradeFunc {
private:
  double _x0;
  double _x1;
  double _x01;

public:
  /** Constructor */
  inline reverseGradeFunc(double x0, double x1)
      : _x0(x0), _x1(x1), _x01(x1 - x0) {}

  /** Destructor */
  inline ~reverseGradeFunc() {}

  /** Apply function */
  inline bool operator()(double x) const {
    if (x <= _x0)
      return 1.;
    if (x >= _x1)
      return 0.;
    return (_x1 - x) / _x01;
  }
};

/** Triangular membership function object */
class triangularFunc {
private:
  double _x0;
  double _x1;
  double _x2;
  double _x01;
  double _x12;

public:
  /** Constructor */
  inline triangularFunc(double x0, double x1, double x2)
      : _x0(x0), _x1(x1), _x2(x2), _x01(x1 - x0), _x12(x2 - x1) {}

  /** Destructor */
  inline ~triangularFunc() {}

  /** Apply function */
  inline bool operator()(double x) const {
    if (x <= _x0)
      return 0.;
    if (x > _x0 && x <= _x1)
      return (x - _x0) / _x01;
    return (_x2 - x) / _x12;
  }
};

/** Trapezoid membership function object */
class trapezoidFunc {
private:
  double _x0;
  double _x1;
  double _x2;
  double _x3;
  double _x01;
  double _x23;

public:
  /** Constructor */
  inline trapezoidFunc(double x0, double x1, double x2, double x3)
      : _x0(x0), _x1(x1), _x2(x2), _x3(x3), _x01(x1 - x0), _x23(x3 - x2) {}

  /** Destructor */
  inline ~trapezoidFunc() {}

  /** Apply function */
  inline bool operator()(double x) const {
    if (x <= _x0)
      return 0.;
    if (x >= _x1 && x <= _x2)
      return 1.;
    if (x > _x0 && x < _x1)
      return (x - _x0) / _x01;
    return (_x3 - x) / _x23;
  }
};

#endif
