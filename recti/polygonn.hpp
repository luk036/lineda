#ifndef RECTI_POLYGONN_HPP
#define RECTI_POLYGONN_HPP

namespace recti {

template <typename _Tp> class Polygonn;
template <typename _Tp> class Pointer;
template <typename _Tp> class Rectlink;

/** Polygon for polygon cutting.
 *
 *  Claim 1: Every simply axis-parallel polygon admits a polygon
 *cutting, and any cutting of a simple axis-parallel polygon with n
 *vertices consists of at most $(n-2)/2$ rectangles.
 *  Proof by induction on $n$. When $n = 4$ the polygon itself is a
 *rectangle and the theorem is trivially true. Let $n > 4$, and assume
 *that the theorem is true for all $m < n$. Let $P$ be a polygon with
 *$n$ vertices. We first prove the existence of a ``cut line'' in
 *$P$. Let $v$ be the leftmost vertical line segment of $P$. (In case
 *of ties, we take the lowest leftmost segment.) Let u and w be the
 *two neighboring segments of $v$ on the boundary of $P$. If the open
 *segment uw lies in the interior of $P$, we have found a cut line.
 *Otherwise, there are one or more those vertical segments, let v' be the
 *one farthest from uw. The horizontal segment connecting v' to v
 *cannot intersect an edge of $P$, because such an edge would have an
 *endpoint inside the rectangle that is farther from uw, contradicting
 *the definition of v'. Hence vv' is a cut.
 *  So a cut exists. Any cut line cuts $P$ into two simple subpolygons
 *$P1$ and $P2$. Let $m1$ be the number of vertices of $P1$ and $m2$
 *the number of vertices of $P2$. Both $m1$ and $m2$ must be smaller
 *than $n$, so by induction $P1$ and $P2$ can be decomposed. Hence,
 *$P$ can be decomposed as well.
 */
template <class _Tp> class Polygonn {
  typedef Pointer<_Tp> _Node;

public:
  Pointer<_Tp> *_head;
  int _pointernum;
  int _rectanglenum;
  Pointer<_Tp> *_rectanglegroup[20000];
  Rectlink<_Tp> *_rectanglestack[20000];

public:
  Polygonn(Pointer<_Tp> *head, int pointernum, int _rectanglenum);

  static Pointer<_Tp> *leftdown(Pointer<_Tp> *head);
  Pointer<_Tp> *normalize(Pointer<_Tp> *head);

  void cut(Pointer<_Tp> *head);
  void enlarge(_Tp index);
  _Tp area(Pointer<_Tp> *head);
  void cparea(_Tp area);
};
}
#endif
