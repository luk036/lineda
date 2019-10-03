#ifndef _EX_QUADRATIC_HPP
#define _EX_QUADRATIC_HPP

/** Example of a quasi-convex function */
template <class Matrix, class Vector> class ex_quadratic {
private:
  size_t _m;
  Matrix _P;
  Vector _c;
  Vector _d;

public:
  ex_quadratic() : _m(1), _P(3, 3), _c(3), _d(3) {
    _P[0][0] = 66;
    _P[0][1] = 78;
    _P[0][2] = 90;
    _P[1][0] = 78;
    _P[1][1] = 93;
    _P[1][2] = 108;
    _P[2][0] = 90;
    _P[2][1] = 108;
    _P[2][2] = 126;
    _c[0] = -1;
    _c[1] = -3;
    _c[2] = -5;
    _d[0] = -1;
    _d[1] = -2;
    _d[2] = -3;
  }

  ~ex_quadratic() {}

  size_t num_constraints() { return _m; }

  double operator()(const Vector &x, int j) const {
    double res = -1000;
    Vector temp(3), temp2(3);

    switch (j) {
    case 0: // objective function
      // res = 0.5 * (x+c)'*P*(x+c)
      copy(x, temp);
      add(_c, temp);
      mult(_P, temp, temp2);
      res = 0.5 * dot(temp, temp2);
      break;
    case 1: // first constraint function
      res = dot(_d, x) + 8;
      break;
    }
    return res;
  }

  void gradient(Vector &g, const Vector &x, int j) const {
    Vector temp(3);

    switch (j) {
    case 0: // objective function
      // g = P*(x+c)
      copy(x, temp);
      add(_c, temp);
      mult(_P, temp, g);
      break;
    case 1: // first constraint function
      copy(_d, g);
      break;
    }
  }
}; // END of quadratic

#endif
