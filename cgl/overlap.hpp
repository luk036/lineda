#ifndef CGL_OVERLAP_HPP
#define CGL_OVERLAP_HPP

#include "sweeper.hpp"
#include <intv_multimap.hpp>
#include <rectifwd.hpp>
#include <vector>

namespace cgl {
/**
 * @addtogroup cgl
 * @{
 */
struct point_event; // forward declare

/** Event info */
struct event_info {
  typedef std::intv_multimap<int, event_info *> Emap;
  const recti::Rectangle<int>
      *_data;         /**< rectanlge that the event point belong to */
  Emap::iterator _it; /**< for easing the intervals */
};

/** Point event for plane sweeping algorithm. An event happen when
    the sweep line encounters the corner points of rectangles. */
struct point_event {
  bool _is_start;    /**< whether it is a start point */
  int _x;            /**< x coordinate */
  event_info *_info; /**< Event info */

  /** @return whether this event has higher priority than rhs */
  inline bool operator<(const point_event &rhs) const {
    return _x > rhs._x || (_x == rhs._x && !_is_start);
  }
};

class todoEvent {
private:
  typedef std::intv_multimap<int, event_info *> Emap;
  Emap _S; // for detecting interval overlapping

protected:
  size_t _enlarge_factor;

public:
  todoEvent(size_t enlarge_factor = 0) : _enlarge_factor(enlarge_factor){};
  virtual ~todoEvent() = default;
  ;

  void operator()(point_event &ptE);
  virtual void do_start_event(event_info &, event_info &) {}
  virtual void do_end_event(event_info &) {}
};

/** Overlap sweeping algorithm for overlap detection of a set
    of rectangles */
class overlap_sweeper {
  typedef boost::signal<void(point_event e)> OnEvent;

private:
  std::vector<event_info> _event_infos;   /**< event information */
  std::vector<point_event> _point_events; /**< point events */
  sweeper<point_event> *_sweeper;
  size_t _enlarge_factor;

public:
  /** Constructor */
  overlap_sweeper(const std::vector<recti::rectangle> &rects,
                  size_t enlarge_factor = 0);

  /** Destructor */
  ~overlap_sweeper();

  /** Connect slot */
  void do_on_event(const OnEvent::slot_type &slot) {
    _sweeper->do_on_event(slot);
  }

  /** Overlap detection */
  void scan() { _sweeper->scan(); }
};
}

#endif
