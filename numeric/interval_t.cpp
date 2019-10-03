#include "interval.hpp"
#include <iostream>
#include <set>

using namespace std;
using namespace numeric;

int main() {
  const interval<int> p1(4, 6), p2(5, 6), p3(7, 8), p4(9, 10), p5(4, 6),
      p6(2, 2), p7(4, 5);
  multiset<interval<int> > Q;

  Q.insert(p1);
  Q.insert(p2);
  Q.insert(p3);
  Q.insert(p4);
  Q.insert(p5);

  interval<int> q(4, 6);

  typedef set<interval<int> >::const_iterator itv_itr_t;
  const pair<itv_itr_t, itv_itr_t> range = Q.equal_range(q);
  itv_itr_t it1 = range.first;
  const auto it2 = range.second;
  for (; it1 != it2; ++it1) {
    cout << *it1 << endl;
  }
  cout << "p6" << p6
       << " can be degenerated to a single point?:" << p6.is_exact() << endl;
  cout << "p1==p5?:" << (p1 == p5) << "  "
       << "p1!=p2?:" << (p1 != p2) << endl;

  cout << p1 << "<" << p3 << "?:" << (p1 < p3) << endl;
  cout << p1 << "<=" << p3 << "?:" << (p1 <= p3) << endl;
  cout << p1 << ">" << p3 << "?:" << (p1 > p3) << endl;
  cout << p1 << ">=" << p3 << "?:" << (p1 >= p3) << endl;

  cout << p2 << "is left of" << p3 << "?:" << p2.left_of(p3) << endl;
  cout << p3 << "is right of" << p2 << "?:" << p3.right_of(p2) << endl;
  cout << p1 << "contains" << p7 << "?:" << p1.contain(p7) << endl;
  cout << p1 << "contains 5"
       << "?:" << p1.contain(5) << endl;
  cout << p1 << "and " << p3 << "'s hull is" << p1.hull(p3) << endl;
  cout << p1 << "and " << p3 << "'s gap is" << p1.gap(p3) << endl;
  cout << p1 << "and " << p2 << "'s intersection is" << p1.intersect(p2)
       << endl;

  cout << p1 << "*" << p2 << p1 * p2 << endl;
  cout << p1 << "+" << p2 << p1 + p2 << endl;
  cout << p1 << "-" << p2 << p1 - p2 << endl;
  cout << p1 << "/" << p2 << p1 / p2 << endl;

  cout << q << "+=" << p2 << (q += p2) << endl;
  cout << q << "-=" << p2 << (q -= p2) << endl;
  cout << q << "*=" << p2 << (q *= p2) << endl;
  cout << q << "/=" << p2 << (q /= p2) << endl;

  int c = 10;

  cout << q << "*" << c << p1 * c << endl;
  cout << q << "+" << c << p1 + c << endl;
  cout << q << "-" << c << p1 - c << endl;
  cout << q << "/" << c << p1 / c << endl;

  cout << q << "+=" << c << (q += c) << endl;
  cout << q << "-=" << c << (q -= c) << endl;
  cout << q << "*=" << c << (q *= c) << endl;
  cout << q << "/=" << c << (q /= c) << endl;

  q = random();
  cout << q << "lower:" << q.lower() << "upper:" << q.upper() << endl;

  cout << "max(p1,p2):" << max(p1, p2) << endl;
  cout << "min(p1,p2):" << min(p1, p2) << endl;
  cout << "2,3=>" << make_interval(2, 3) << endl;

  cout << p1 << p2 << "max distance:" << max_dist(p1, p2) << endl;
  cout << p1 << p2 << "min distance:" << min_dist(p1, p2) << endl;

  cout << interval<double>(3.123) << endl;
  cout << interval<float>(1.2, 2.3) << endl;
  cout << interval<int>(interval<int>(1, 2)) << endl;
}
