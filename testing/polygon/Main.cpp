#include <polygon.hpp>
#include <svgstream.hpp>
#include <vector>

using namespace recti;

int main(int argc, const char *argv[]) {
  const size_t N = (argc <= 1) ? 10 : atoi(argv[1]);
  // srand((unsigned)time(NULL));

  const Polygon<int> P = Polygon<int>::random(N);

  svgstream test("polygon.svg");
  test << P;
  test << '\n';

  // xxx std::vector<Point<int> > V;
  // xxx V.reserve(N);  // reserve enough memory
  // xxx
  // xxx typedef RPolygon<int>::pt_set_type pt_set_type;
  // xxx typedef pt_set_type::const_iterator const_iterator;
  // xxx const pt_set_type pt_set = P.get_pt_set();
  // xxx const_iterator it = pt_set.begin();
  // xxx do { // traverse circulant linked list
  // xxx   V.push_back(*it); // push a data point to vector
  // xxx } while (++it != pt_set.begin());

  return 0;
}
