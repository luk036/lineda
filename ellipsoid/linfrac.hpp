#ifndef _EX_LINFRAC_HPP
#define _EX_LINFRAC_HPP

/** Example of a quasi-convex function */
template <class Vector> class ex_linfrac {
private:
  Vector a;
  double b;
  Vector c;
  double d;

public:
  ex_linfrac() : a(3), c(3) {
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    b = 4;
    c[0] = 5;
    c[1] = 6;
    c[2] = 7;
    d = 8;
  }

  ~ex_linfrac() {}

  size_t num_constraints() { return 1; }

  double operator()(const Vector &x, int j) const {
    double res = -1000;

    switch (j) {
    case 0: // objective function
      res = (dot(a, x) + b) / (dot(c, x) + d);
      break;
    case 1: // first constraint function
      res = 3. - (dot(c, x) + d);
      break;
    }
    return res;
  }

  void gradient(Vector &g, const Vector &x, int j) const {
    double f;

    switch (j) {
    case 0: // objective function
      f = ex_linfrac<Vector>()(x, 0);
      // copy(a, g);
      add(a, scaled(c, -f), g);
      break;
    case 1: // first constraint function
      copy(c, g);
      scale(g, -1);
      break;
    }
  }
}; // END of linfrac

#endif
