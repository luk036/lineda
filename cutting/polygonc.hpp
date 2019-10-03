/**
 * @file polygonc.hpp
 * @author why(Hanyi Wei)
 * @date Sep.9, 2009
 *
 * @brief used to cut the input polygon and
 * get the array of got rectangles
 * use new cutting method which aims at the angles of
 * 270 and find the minNode to cut and make the angle
 * of 270 to be the angle of 90
 * the cutting is over when no angles of 270 remain
 * and we get the rectangles
 * for no-bevel and no-hole polygon cutting only now
 * permit overlapping cutting
 */
#ifndef CUTTING_POLYGONC_HPP
#define CUTTING_POLYGONC_HPP

#include <vector>

template <typename _Tp> class Polygonc;
template <typename _Tp> class Pointer;

// use std::vector
using namespace std;

template <class _Tp> class Polygonc {
  /** a briefer type of Pointer<_Tp>
   *  not used frequently
   */
  typedef Pointer<_Tp> _Node;

public:
  // int _pointernum;

  /** the number of rectangles which have been in
   *  _rectanglegroup[]
   */
  int _rectanglenum;
  /** an array which contains the got rectangles*/
  Pointer<_Tp> *_rectanglegroup[20000];

  // Rectlink<_Tp>* _rectanglestack[20000 ];

  /** @constructor */
  Polygonc(vector<Pointer<_Tp> > &Pn, int rectanglenum);

  // Pointer<_Tp>* normalize(Pointer<_Tp>*  head);

  /** @the main function of cutting */
  void cut();

  // void enlarge(_Tp index);
  //_Tp area(Pointer<_Tp>* head);
  // void cparea(_Tp area);

private:
  /** the vector which contains the list of pointers
   *  become reference of the input Pn and after cutting
   *  there are only rectangles left in _Pn
   */
  vector<Pointer<_Tp> > &_Pn;
  /** the vector which contains the new nodes created
   *  in createPolygon() during the cutting
   *  after cutting _Pn and _addedPn are considered as
   *  a whole
   */
  vector<Pointer<_Tp> *> _addedPn;

  /** @cut at the position of the current node which
   *  corresponds to an angle of 270
   *  there are 28 small conditions in 4 big conditions
   *  in some conditions new nodes may be created
   *  used in cut()
   */
  void createPolygon(Pointer<_Tp> *curNode, int type);
  /** @find the node in the line which has the min
   *  manhattan distance with the current node and
   *  lies in the quadrant according to the position
   *  of the current node in the polygon
   *  used in createPolygon()
   */
  Pointer<_Tp> *min(Pointer<_Tp> *curNode, int type);
};
#endif
