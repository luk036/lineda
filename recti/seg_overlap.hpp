#ifndef RECTI_SEG_OVERLAP_HPP
#define RECTI_SEG_OVERLAP_HPP

#include "polygon.hpp"
#include "rectifwd.hpp"
#include "segment.hpp"
#include <list>
#include <set>
#include <vector>

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

// Forward declarations
struct event;
struct event_wp;
struct event_info_wp;

/** Event info */
struct event_info {
  typedef std::list<event_info_wp>::iterator iterator;

  event *_start; /**< backward pointer to start event */
  event *_end;   /**< backward pointer to end event */
  segment _seg;  /**< segment corresponding to start and end point */
  iterator _it;  /**< for erasing this event info */

  inline bool operator==(const event_info &rhs) const {
    return !(*this < rhs || rhs < *this);
  }

  /** Note: this operation is not transitive */
  inline bool operator<(const event_info &rhs) const {
    const segment &rseg = rhs._seg;
    // return _seg.left_of(rseg) || rseg.right_of(_seg);
    int a1 = area2(_seg.start(), _seg.end(), rseg.start());
    int a2 = area2(_seg.start(), _seg.end(), rseg.end());
    if (a1 > 0 && (a2 > 0 || _seg.end() == rseg.end()))
      return true;
    if (a2 > 0 && (a1 > 0 || _seg.start() == rseg.start()))
      return true;

    int a3 = area2(rseg.start(), rseg.end(), _seg.start());
    int a4 = area2(rseg.start(), rseg.end(), _seg.end());
    if (a3 < 0 && (a4 < 0 || _seg.end() == rseg.end()))
      return true;
    if (a4 < 0 && (a3 < 0 || _seg.start() == rseg.start()))
      return true;
    return false;
  }
};

/** Point event for plane sweeping algorithm. An event happen when
    the sweep line encounters the corner points of rectangles. */
struct event {
  typedef polygon::pnode pnode;
  typedef std::multiset<event_wp>::iterator iterator;

  bool _is_start;    /**< whether it is a start point */
  point _p;          /**< coordinate */
  event_info *_info; /**< Event info */
  pnode *_node;      /**< Node pointer */
  // xxx iterator       _it;            /**< for erasing the event */

  /** @return whether this event has higher priority than rhs */
  inline bool operator<(const event &rhs) const {
    return (_p.y() < rhs._p.y()) ||
           (_p.y() == rhs._p.y() && _p.x() < rhs._p.x()) ||
           (_p.y() == rhs._p.y() && _p.x() == rhs._p.x() && !_is_start);
  }
};

/** Wrapper for event for std::multiset<> */
struct event_wp {
  event *_impl; /** actual implementation of event */

  /** Constructor */
  event_wp(event &Impl) : _impl(&Impl) {}

  /** @return whether this event has higher priority than rhs */
  inline bool operator<(const event_wp &rhs) const {
    return *_impl < *rhs._impl;
  }
};

/** Wrapper for event_info for std::list<> */
struct event_info_wp {
  event_info *_impl; /** actual event_info implementation */

  /** Constructor */
  event_info_wp(event_info &Impl) : _impl(&Impl) {}

  /** @return whether this event_info has higher priority than rhs */
  inline bool operator==(const event_info_wp &rhs) const {
    return *_impl == *rhs._impl;
  }

  /** @return whether this event_info has higher priority than rhs */
  inline bool operator<(const event_info_wp &rhs) const {
    return *_impl < *rhs._impl;
  }
};

/** Line segment intersection detection for polygon random
    generation. Plane sweeping algorithm we use here is described in
    the book "Computational Geometry: Algorithms and Applications" */
class seg_overlap_sweeper {
  typedef polygon::pnode pnode;
  typedef polygon::pt_set_type pt_set_type;

private:
  std::vector<event_info> _event_infos; /**< event information */
  std::vector<event> _events;           /**< point events */
  std::multiset<event_wp> _event_set;   /**< event set */
  std::list<event_info_wp> _S;          /**< keep track status */

public:
  /** Constructor */
  seg_overlap_sweeper(polygon &P);

  /** Overlap detection */
  bool scan();
};
}

#endif
