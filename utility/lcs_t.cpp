#include <boost/bind.hpp>
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <string>

class EditDistance {
  typedef boost::multi_array<int, 2> array2d;
  typedef boost::multi_array<boost::mutex *, 2> arraymutex2d;
  typedef array2d::index index;
  // typedef arraymutex2d::index mindex;

public:
  /** Constructor */
  EditDistance(const std::string &X, const std::string &Y)
      : _X(X), _Y(Y), _m(X.length()), _n(Y.length()),
        _D(boost::extents[_m + 1][_n + 1]),
        _io_mutexes(boost::extents[_m + 1][_n + 1]) {
    for (int i = 1; i <= _m; ++i)
      for (int j = 1; j <= _n; ++j) {
        _io_mutexes[i][j] = new boost::mutex();
        _D[i][j] = -1; // not evaluated yet
      }
    for (int i = 0; i <= _m; ++i)
      _D[i][0] = i; // deletion
    for (int j = 0; j <= _n; ++j)
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

  int operator()() {
    calc_dist(_m, _n);
    return _D[_m][_n];
  }

  int value() {
    calc_dist(_m, _n);
    return _D[_m][_n];
  }

private:
  void calc_dist(index i, index j) {
    if (_D[i][j] != -1)
      return; // already calculated

    {
      boost::mutex::scoped_lock lock(*_io_mutexes[i][j]);
      if (_D[i][j] != -1)
        return; // double checked

      if (_X[i - 1] == _Y[j - 1]) {
        calc_dist(i - 1, j - 1);
        _D[i][j] = _D[i - 1][j - 1];
      } else {
        // calc_dist(i-1,j);
        // calc_dist(i,j-1);
        // calc_dist(i-1,j-1);

        boost::thread th1(
            boost::bind(&EditDistance::calc_dist, this, i - 1, j));
        boost::thread th2(
            boost::bind(&EditDistance::calc_dist, this, i, j - 1));

        calc_dist(i - 1, j - 1);

        th1.join();
        th2.join();
        _D[i][j] =
            std::min(_D[i - 1][j - 1], std::min(_D[i][j - 1], _D[i - 1][j])) +
            1;
      }
    }
    return;
  }

private:
  const std::string &_X;
  const std::string &_Y;
  index _m;
  index _n;
  array2d _D;
  arraymutex2d _io_mutexes;
};

int main() {
  std::string X("thix is a test");
  std::string Y("this is aa test");

  EditDistance E(X, Y);
  std::cout << E.value() << std::endl;

  // std::cout << EditDistance(Y,X)() << std::endl;
}
