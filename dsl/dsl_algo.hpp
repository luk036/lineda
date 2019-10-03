// Algorithm implementation -*- C++ -*-

/** @file dsl_algo.hpp
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef DSL_INTERNAL_ALGO_H
#define DSL_INTERNAL_ALGO_H

#include <boost/concept_check.hpp>
#include <iterator>

namespace dsl {

/**
 *  @brief Apply a function to every element of a cyclic sequence.
 *  @param  __first  An input iterator.
 *  @param  __f      A unary function object.
 *  @return   @p __f.
 *
 *  Applies the function object @p __f to each element in the range
 *  @p [first,last).  @p __f must not modify the order of the sequence.
 *  If @p __f has a return value it is ignored.
*/
template <typename _InputIter, typename _Function>
inline _Function for_each_in_cycle(_InputIter __first, _Function __f) {
  // concept requirements
  // boost::function_requires(boost::InputIteratorConcept<_InputIter>());
  const _InputIter __last = __first;
  do
    __f(*__first);
  while (++__first != __last);
  return __f;
}

/**
 *  @brief Assign the result of a function object to each value in a
 *         sequence.
 *  @param  __first  A forward iterator.
 *  @param  __last   A forward iterator.
 *  @param  __gen    A function object taking no arguments.
 *  @return   generate() returns no value.
 *
 *  Performs the assignment @c *i = @p gen() for each @c i in the range
 *  @p [first,last).
*/
template <typename _ForwardIter, typename _Generator>
void generate_in_cycle(_ForwardIter __first, _Generator __gen) {
  // concept requirements
  // boost::function_requires(boost::ForwardIteratorConcept<_ForwardIter>() );
  // boost::function_requires(boost::GeneratorConcept<_Generator,
  // typename std::iterator_traits<_ForwardIter>::value_type>() );

  const _ForwardIter __last = __first;
  do
    *__first = __gen();
  while (++__first != __last);
}

/** min_element and max_element, with and without an explicitly supplied
    comparison function. */
template <typename _ForwardIter>
_ForwardIter max_element_in_cycle(_ForwardIter __first) {
  // concept requirements
  // boost::function_requires(boost::ForwardIteratorConcept<_ForwardIter>());
  // boost::function_requires(boost::LessThanComparableConcept<
  // typename std::iterator_traits<_ForwardIter>::value_type>());

  const _ForwardIter __last = __first;
  _ForwardIter __result = __first;
  while (++__first != __last)
    if (*__result < *__first)
      __result = __first;
  return __result;
}

/** min_element and max_element, with and without an explicitly supplied
    comparison function. */
template <typename _ForwardIter, typename _Compare>
_ForwardIter max_element_in_cycle(_ForwardIter __first, _Compare __comp) {
  // concept requirements
  // boost::function_requires(boost::ForwardIteratorConcept<_ForwardIter>());
  // boost::function_requires(boost::BinaryPredicateConcept<_Compare,
  //  typename std::iterator_traits<_ForwardIter>::value_type,
  //  typename std::iterator_traits<_ForwardIter>::value_type>() );

  const _ForwardIter __last = __first;
  _ForwardIter __result = __first;
  while (++__first != __last)
    if (__comp(*__result, *__first))
      __result = __first;
  return __result;
}

/** min_element and max_element, with and without an explicitly supplied
    comparison function. */
template <typename _ForwardIter>
_ForwardIter min_element_in_cycle(_ForwardIter __first) {
  // concept requirements
  // boost::function_requires(boost::ForwardIteratorConcept<_ForwardIter>());
  // boost::function_requires(boost::LessThanComparableConcept<
  //  typename std::iterator_traits<_ForwardIter>::value_type>() );

  const _ForwardIter __last = __first;
  _ForwardIter __result = __first;
  while (++__first != __last)
    if (*__first < *__result)
      __result = __first;
  return __result;
}

/** min_element and max_element, with and without an explicitly supplied
    comparison function. */
template <typename _ForwardIter, typename _Compare>
_ForwardIter min_element_in_cycle(_ForwardIter __first, _Compare __comp) {
  // concept requirements
  // boost::function_requires(boost::ForwardIteratorConcept<_ForwardIter>());
  // boost::function_requires(boost::BinaryPredicateConcept<_Compare,
  //    typename std::iterator_traits<_ForwardIter>::value_type,
  //    typename std::iterator_traits<_ForwardIter>::value_type>() );

  const _ForwardIter __last = __first;
  _ForwardIter __result = __first;
  while (++__first != __last)
    if (__comp(*__first, *__result))
      __result = __first;
  return __result;
}

} // namespace dsl

#endif
