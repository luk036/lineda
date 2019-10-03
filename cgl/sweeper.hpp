#ifndef CGL_SWEEPER_HPP
#define CGL_SWEEPER_HPP

#include <boost/signal.hpp>
#include <boost/signals/slot.hpp>
#include <queue>
#include <vector>

namespace cgl {
/**
 * @addtogroup cgl
 * @{
 */

/** Plane sweeping algorithm for overlap detection of a set of
    rectangles */
template <class Event, class Compare = std::less<Event> > class sweeper {
  typedef boost::signal<void(Event e)> OnEvent;
  typedef std::priority_queue<Event, std::vector<Event>, Compare> PQ;

private:
  PQ _pq;            /**< event priority queue */
  OnEvent _on_event; /**< on Event signal */

public:
  /** Constructor */
  template <typename ForwardIterator>
  sweeper(ForwardIterator first, ForwardIterator last) {
    while (first != last) {
      _pq.push(*first);
      ++first;
    }
  }

  /** Connect slot */
  void do_on_event(const typename OnEvent::slot_type &slot) {
    _on_event.connect(slot);
  }

  /** perform scan line */
  void scan() {
    while (!_pq.empty()) {
      _on_event(_pq.top());
      _pq.pop();
    }
  }
};
}

#endif
