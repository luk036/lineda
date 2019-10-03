/**
 * @file pointer.hpp
 * @author why(Hanyi Wei)
 * @date Sep.8, 2009
 *
 * @brief each pointer corresponds to each node
 * nodes constructed as a bidirectional list
 */
#ifndef CUTTING_POINTER_HPP
#define CUTTING_POINTER_HPP

template <typename _Tp> class Pointer {
public:
  /** @constructor
   *  in both constructors isRefered is made false
   *  in the second constructor only the coordinate of p is copied
   */
  Pointer(_Tp x, _Tp y, Pointer<_Tp> *next, Pointer<_Tp> *prev);
  Pointer(Pointer<_Tp> *p);

  /** the coordinate of each node */
  _Tp _x, _y;

  /** the pointers of next and prevenient node */
  Pointer<_Tp> *_next;
  Pointer<_Tp> *_prev;

  /** a symbol which refers to whether the node is in
   *  the got rectangles */
  bool _isRefered;

  /** @return whether the two nodes have equal coordinate */
  bool equal(Pointer<_Tp> *node);

  /** @set _next as next */
  void next(Pointer<_Tp> *next = 0);
  /** @set _prev as prev */
  void prev(Pointer<_Tp> *prev = 0);

  /** @return whether the line of the node and next node
   *  is vertical */
  bool nextvertical();
  /** @return whether the line of the node and next node
   *  is horizontal */
  bool nexthorizontal();
  /** @return whether the line of the node and prevenient
   *  node is vertical */
  bool prevvertical();
  /** @return whether the line of the node and prevenient
   *  node is horizontal */
  bool prevhorizontal();
  bool nextdiagonal();
  bool prevdiagonal();

  /** @print the coordinate of the current point */
  void print();
};

// template<typename _Tp , typename _Up>
// inline bool operator==(Pointer<_Tp>* node1,Pointer<_Up>* node)
//{return node1->_x == node2->_x && node1->_y == node2->_y;}

//}
#endif
