#ifndef NUMERIC_FWD_HPP
#define NUMERIC_FWD_HPP

/** Forward declarations */
namespace numeric {

template <typename _Tp> class interval;
template <typename _Tp> struct vector2;
template <typename _Tp> struct matrix2;
template <typename _Tp> class polynomial;
template <typename _Tp> class triple;

template <typename _Tp> inline interval<_Tp> make_interval(_Tp x, _Tp y);

class bad_polynomial;
}
#endif
