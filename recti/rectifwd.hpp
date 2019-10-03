#ifndef RECTI_FWD_HPP
#define RECTI_FWD_HPP

/** Forward declarations of recti classes */
namespace recti {
template <class _Tp> class checker;
template <typename _Tp> class Coord_sys;
class orient;
template <typename _Tp> class Path;
template <typename _Tp> class Point;
template <typename _Tp> class Polygon;
template <typename _Tp> class Pointer;
template <typename _Tp> class Point_set;
template <typename _Tp> class Polygonn;
template <typename _Tp> class RPolygon;
template <typename _Tp> class Rectangle;

typedef Coord_sys<int> coord_sys;
typedef Path<int> path;
typedef Point<int> point;
typedef Point_set<int> point_set;
typedef Polygon<int> polygon;
typedef RPolygon<int> rpolygon;
typedef Rectangle<int> rectangle;
}

#endif
