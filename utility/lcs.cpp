#include <boost/multi_array.hpp>
#include <iostream>
#include <string>

class EditDistance {
  typedef boost::multi_array<int, 2> array2d;
  typedef array2d::index index;

public:
  /** Constructor */
  EditDistance(const std::string &X, const std::string &Y)
      : _X(X), _Y(Y), _m(X.length()), _n(Y.length()),
        _D(boost::extents[_m + 1][_n + 1]) {
    for (index i = 1; i <= _m; ++i)
      for (index j = 1; j <= _n; ++j)
        _D[i][j] = -1; // not evaluated yet
    for (index i = 0; i <= _m; ++i)
      _D[i][0] = i; // deletion
    for (index j = 0; j <= _n; ++j)
      _D[0][j] = j; // insertion
  }

  ~EditDistance() {
    for (index i = 0; i <= _m; ++i) {
      std::cout << "|";
      for (index j = 0; j <= _n; ++j) {
        std::cout << " " << _D[i][j] << " |";
      }
      std::cout << std::endl;
    }
  }

  int operator()() { return dist(_m, _n); }

  int value() { return dist(_m, _n); }

private:
  int dist(index i, index j) {
    if (_D[i][j] != -1)
      return _D[i][j]; // already calculated
    if (_X[i - 1] == _Y[j - 1]) {
      _D[i][j] = dist(i - 1, j - 1);
    } else {
      int del = dist(i - 1, j);
      int ins = dist(i, j - 1);
      int sub = dist(i - 1, j - 1);
      _D[i][j] = std::min(del, std::min(ins, sub)) + 1;
    }
    return _D[i][j];
  }

private:
  const std::string &_X;
  const std::string &_Y;
  index _m;
  index _n;
  array2d _D;
};

int main() {
  std::string X("this is a test");
  std::string Y("this is aa test");

  EditDistance E(X, Y);
  std::cout << E.value() << std::endl;

  std::cout << EditDistance(Y, X)() << std::endl;
}
