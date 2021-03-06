#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <intv_set.hpp>
#include <iostream>
#include <pst_tree.hpp>
#include <set>
#include <svgstream.hpp>
#include <vector>

// xxx using namespace dsl;
using namespace std;
using namespace boost::lambda;

// xxx int dummy()
// xxx {
// xxx   //xxx int s[] = {2, 3, 5, 8, 13, 21, 34, 45, 67, 81};
// xxx   //xxx int s[] = {81, 67, 45, 34, 21, 13, 8, 5, 3, 2};
// xxx    int s[] = {33, 32, 2, 5, 3,  6,  9,  3,  4, 45, 43, 76, 23, 11};
// xxx    int t[] = {3,  2,  2, 5, 33, 36, 29, 33, 4, 45, 43, 6,  3,  1};
// xxx    size_t N = sizeof(s)/sizeof(int);
// xxx    set<int> S1(s, s+N);
// xxx    intv_set<int> S2(s, s+N, t);
// xxx
// xxx    //xxx S1.erase(5);
// xxx    //xxx S1.erase(3);
// xxx    //xxx S1.erase(33);
// xxx    for_each(S1.begin(), S1.end(), cout << _1 << " ");
// xxx    cout << endl;
// xxx
// xxx    //xxx intv_multiset<int>::iterator it3 = S2.insert(45,7);
// xxx    S2.erase(5, 5);
// xxx    S2.erase(11, 1);
// xxx    //xxx S2.erase(it3);
// xxx    //xxx S2.erase(2, 2);
// xxx    //xxx S2.erase(33);
// xxx    intv_set<int>::iterator it = S2.lower_bound(2, 10);
// xxx    //xxx intv_set<int>::iterator it2 = S2.upper_bound(49, 10);
// xxx    for (; it != S2.end(); ++it) {
// xxx      cout << it.x() << "," << it.y() << " ";
// xxx    }
// xxx    //xxx for_each(S2.begin(), S2.end(), cout << _1 << " ");
// xxx    cout << endl;
// xxx    return 0;
// xxx }

int main() {
  // xxx int s[] = {2, 3, 5, 8, 13, 21, 34, 45, 67, 81};
  // xxx int s[] = {81, 67, 45, 34, 21, 13, 8, 5, 3, 2};
  // xxx int s[] = {33, 32, 2, 5, 3,  6,  9,  3,  4, 45, 43, 76, 23, 11};
  // xxx int t[] = {3,  2,  2, 5, 33, 36, 29, 33, 4, 45, 43, 6,  3,  1};

  size_t N = 10;
  vector<int> s(N);
  vector<int> t(N);
  std::generate(s.begin(), s.end(), rand);
  std::generate(t.begin(), t.end(), rand);
  std::for_each(s.begin(), s.end(), _1 = _1 % 2000 - 1000);
  std::for_each(t.begin(), t.end(), _1 = _1 % 2000 - 1000);

  set<int> S1(s.begin(), s.end());
  intv_set<int> S2(s.begin(), s.end(), t.begin());

  // xxx intv_multiset<int>::iterator it3 = S2.insert(45,7);
  // xxx S2.insert(make_pair(45, 3.3), 7);
  // xxx S2.insert(make_pair(45, 3.4), 7);
  // xxx S2.erase(it3);
  // xxx intv_set<int, double>::iterator it = S2.lower_bound(2, 200);
  // xxx //xxx intv_set<int>::iterator it = S2.begin();
  // xxx for (; it != S2.end(); ++it) {
  // xxx   cout << it.x().first << "," << it.x().second << "," << it.y() << " ";
  // xxx }

  // xxx cout << endl;
  svgstream tout("pst.svg");
  tout << S2 << std::endl;
}
