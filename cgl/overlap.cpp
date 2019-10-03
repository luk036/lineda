#include "overlap.hpp"
#include <interval.hpp>
#include <rectangle.hpp>
#include <set>
#include <vector>

using namespace std;
using recti::rectangle;
using numeric::interval;

namespace cgl {

/** ??? @todo: add template <typename _Tp> */
void todoEvent::operator()(point_event &ptE) {
  if (ptE._is_start) { // is end point
    interval<int> q = ptE._info->_data->y_bound();
    q += interval<int>(-_enlarge_factor, _enlarge_factor);
    Emap::iterator Sit = _S.lower_bound(q.lower(), q.upper());
    while (Sit != _S.end()) {
      do_start_event(*Sit->second, *ptE._info);
      // throw *ptE._info
      ++Sit;
    }
    // ptE._info->_it = S.insert(make_pair(q.lower(),&ptE), q.upper());
    ptE._info->_it = _S.insert(make_pair(q.upper(), ptE._info), q.lower());
  } else {
    do_end_event(*ptE._info);
    _S.erase(ptE._info->_it);
  }
}

/** Constructor */
overlap_sweeper::overlap_sweeper(const vector<rectangle> &rects,
                                 size_t enlarge_factor)
    : _event_infos(rects.size()), _point_events(2 * rects.size()),
      _enlarge_factor(enlarge_factor) {
  for (size_t i = 0, j = 0; i < rects.size(); ++i, j += 2) {
    _event_infos[i]._data = &rects[i];

    _point_events[j]._info = &_event_infos[i];
    _point_events[j]._is_start = true;
    _point_events[j]._x = rects[i].llx() - enlarge_factor;
    _point_events[j + 1]._info = &_event_infos[i];
    _point_events[j + 1]._is_start = false;
    _point_events[j + 1]._x = rects[i].urx() + enlarge_factor;
  }

  _sweeper =
      new sweeper<point_event>(_point_events.begin(), _point_events.end());
}

overlap_sweeper::~overlap_sweeper() { delete _sweeper; }

/** Return if rectangles that are overlapped. The algorithm takes
    O(N log N) times where N is the number of rectangles. It uses
    the overlap sweeping algorithm that sweeps through
    horizontally. For each particular sweeping line, a set of
    vertical intervals is constructed in order to detect the overlapping. */
// xxx void overlap_sweeper::scan() const
// xxx {
// xxx   mys.do_on_event(todoEvent());
// xxx   mys.scan();
// xxx }

} // namespace
