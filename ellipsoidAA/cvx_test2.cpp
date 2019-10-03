#include "ellipsoid.h"
#include <iostream>
#include <valarray>

/**
    Problem 1: (unconstrainted)
    min  0.5*((x[0]-0.1)^2 + (x[1]-0.2)^2)
*/
class Prob1 {
public:
  using Vec = std::valarray<double>;

  Prob1() : _gradient(Vec(2)){};
  ~Prob1() = default;

  bool is_violated() const {
    return false;
  } // always false because uncontrained
  double f_value() const { return _f_value; }
  const Vec &subgradient() const { return _gradient; }

  void assess(const std::valarray<double> &x) {
    //_f_value = (x[0] - 0.1)*(x[0] - 0.1) + (x[1] - 0.2)*(x[1] - 0.2);
    _gradient[0] = x[0] - 0.1;
    _gradient[1] = x[1] - 0.2;
  }

private:
  double _f_value;
  Vec _gradient;
};

// xxx #include <vector>
/**
  Problem 2:
  min  0.5*((x[0]-0.1)^2 + (x[1]-0.2)^2)
  s.t. x[0] +  x[1] + 3 <= 0
*/
class Prob2 {
public:
  using Vec = std::valarray<double>;

  Prob2() : _gradient(Vec(2)){};
  ~Prob2() = default;

  bool is_violated() const { return _is_violated; }
  double f_value() const { return _f_value; }
  const Vec &subgradient() const { return _gradient; }

  void assess(const std::valarray<double> &x) {
    double f_value = x[0] + x[1] + 3;
    if (f_value > 0) {
      _f_value = f_value;
      _gradient[0] = 1;
      _gradient[1] = 1;
      _is_violated = true;
      return;
    }

    //_f_value = (x[0] - 0.1)*(x[0] - 0.1) + (x[1] - 0.2)*(x[1] - 0.2);
    _gradient[0] = x[0] - 0.1;
    _gradient[1] = x[1] - 0.2;
    _is_violated = false;
  }

private:
  double _f_value;
  Vec _gradient;
  bool _is_violated;
};

int main() {
  using Vec = std::valarray<double>;

  Prob1 P;
  const size_t NUM_VARS = 2;

  // Initial x
  Vec x(NUM_VARS);
  x[0] = x[1] = 0.0;

  // Initial enclosing E
  ellipsoid<Vec> E(x, 20);

  STATUS status = bisection_algo(E, P, x, 1000, 1e-4);
  if (status == FOUND) {
    std::cout << "(" << x[0] << ", " << x[1] << ")" << std::endl;
  }

  return 0;
}
