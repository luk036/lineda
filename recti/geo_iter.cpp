#include "geo_iter.hpp"
#include "polygon.hpp"

namespace recti {

void example_geo_iterator() {
  Polygon<int>::const_iterator it;
  geo_iterator<Polygon<int>::const_iterator> p(it);
  p.x();
}
}
