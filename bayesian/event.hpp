#ifndef EVENT_HPP
#define EVENT_HPP

#include <boost/call_traits.hpp> // for boost::call_traits
#include <boost/config.hpp>      // for BOOST_NO_STDC_NAMESPACE, BOOST_MSVC
#include <cmath>
#include "event.hpp"
#include <vector> // event is implemented by vector

namespace lineda {
/**
 * @addtogroup lineda
 * @{
 */

/** Forward declaration */
template <typename _Tp> class event;

/**
 * Random variable
 */
template <typename _Tp> class event {
  typedef event<_Tp> _Self;
  typedef typename boost::call_traits<_Tp>::param_type param_type;

public:
  /** Default constructor. Create a zero event. */
  inline event() : _prob(0.) {}

  /** Construct a event with probability c */
  inline event(double c) : _prob(c) {}

  // Computer generated copy constructor and operator=() are fine.

  // logical operators

  /** NOT */
  inline event<_Tp> operator!() const { return event<_Tp>(1 - _prob); }

  /** AND */
  template <typename _Up>
  inline event<_Tp> operator&&(const event<_Up> &p) const {
    return event<_Tp>(_prob * p._prob);
  }

  /** OR */
  template <typename _Up>
  inline event<_Tp> operator||(const event<_Up> &p) const {
    return event<_Tp>(1 - (1 - _prob) * (1 - p._prob));
  }

  /** Conditional */
  template <typename _Up>
  inline event<_Tp> operator|(const event<_Up> &q) const {
    return event<_Tp>(_prob);
  }

  double probability() const { return _prob; }

private:
  double _prob;
};

template <typename _Tp> inline double Probability_of(const event<_Tp> &e) {
  return e._prob;
}

/** @} */
}
#endif // EVENT_HPP
