#include "seg_overlap.hpp"
#include "rectangle.hpp"
#include <interval.hpp>
#include <set>
#include <vector>

using namespace std;

namespace recti {

/** Constructor */
seg_overlap_sweeper::seg_overlap_sweeper(polygon &P)
    : _event_infos(P.num_vertices()), _events(2 * P.num_vertices()) {
  typedef polygon::pt_set_type::iterator pt_iter;
  typedef polygon::pnode pnode;

  size_t i = 0;
  size_t j = 0;
  pt_iter it = P._pt_set.begin();

  do {
    pnode *start_pt = it.value();
    ++it;
    pnode *end_pt = it.value();
    if (start_pt->_data.y() > end_pt->_data.y() ||
        (start_pt->_data.y() == end_pt->_data.y() &&
         start_pt->_data.x() > end_pt->_data.x()))
      std::swap(start_pt, end_pt); // make the segment upward oriented

    _event_infos[i]._start = &_events[j];
    _event_infos[i]._end = &_events[j + 1];
    _event_infos[i]._seg = segment(start_pt->_data, end_pt->_data);
    _event_infos[i]._it = _S.end();

    _events[j]._info = &_event_infos[i];
    _events[j]._is_start = true;
    _events[j]._node = start_pt;
    _events[j]._p = start_pt->_data;
    _event_set.insert(event_wp(_events[j]));

    _events[j + 1]._info = &_event_infos[i];
    _events[j + 1]._is_start = false;
    _events[j + 1]._node = end_pt;
    _events[j + 1]._p = end_pt->_data;
    _event_set.insert(event_wp(_events[j + 1]));

    i += 1;
    j += 2;
  } while (it != P._pt_set.begin());
}

/** Perform 2-opt move for each pair of intersecting line segments
    scanned */
bool seg_overlap_sweeper::scan() {
  typedef event::iterator iterator;
  typedef event_info::iterator iterator2;
  bool intersected = false;

  auto it = _event_set.begin();
  _S.clear();

  for (; it != _event_set.end(); ++it) {

    event *ea = it->_impl;
    event_info *info1 = ea->_info;
    if (ea->_is_start) { // is start point
                         // xxx iterator2 it2 = _S.find(event_info_wp(*info1));
      auto it2 = std::find(_S.begin(), _S.end(), event_info_wp(*info1));
      if (it2 == _S.end())
        // xxx info1->_it = _S.insert(event_info_wp(*info1));
        info1->_it = _S.insert(_S.end(), event_info_wp(*info1));
      else {
        event_info *info2 = it2->_impl;
        event *eb = info2->_start;
        event *ec = info1->_end;
        event *ed = info2->_end;
        pnode *a = ea->_node;
        pnode *b = eb->_node;
        pnode *c = ec->_node;
        pnode *d = ed->_node;
        intersected = true;
        bool dir_a = a->_next == c;
        bool dir_b = b->_next == d;
        if (dir_a != dir_b) { // no need to erase
          ed->_info = info1;
          ec->_info = info2;
          info1->_end = ed;
          info2->_end = ec;
          info1->_seg = segment(ea->_p, ed->_p);
          info2->_seg = segment(eb->_p, ec->_p);
          if (dir_a)
            std::swap(b, d);
          else
            std::swap(a, c);
        } else {
          ea->_info = info2;
          ed->_info = info1;
          info2->_end = ea;
          info1->_start = ed;
          ea->_is_start = false;
          ed->_is_start = true;
          info2->_seg = segment(eb->_p, ea->_p);
          info1->_seg = segment(ed->_p, ec->_p);
          if (ed->_p.y() > ec->_p.y() ||
              (ed->_p.y() == ec->_p.y() && ed->_p.x() > ec->_p.x())) {
            std::swap(info1->_start, info1->_end);
            std::swap(ec->_is_start, ed->_is_start);
          }
          if (!dir_a) {
            std::swap(a, c);
            std::swap(b, d);
          }
        }

        // 2-opt move
        b->_next = a;
        c->reverse();
        c->_next = d;

        _S.erase(it2);
        info2->_it = _S.end();
      }
    } else { // is end point
      if (info1->_it != _S.end()) {
        _S.erase(info1->_it);
        info1->_it = _S.end();
      }
    }
  }

  return intersected;
}

} // namespace
