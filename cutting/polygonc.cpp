#include "polygonc.hpp"
#include "pointer.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

/** @constructor
 *  copy Pn to _Pn
 */
template <typename _Tp>
Polygonc<_Tp>::Polygonc(vector<Pointer<_Tp> > &Pn, int rectanglenum)
    : _Pn(Pn), _rectanglenum(rectanglenum) {}

/** @find the node in the line which has the min
 *  manhattan distance with the current node and
 *  lies in the quadrant according to the position
 *  of the current node in the polygon
 *  types could be 1,2,3,4 which correspond to four
 *  conditions of the current node
 *  used in createPolygon()
 */
template <typename _Tp>
Pointer<_Tp> *Polygonc<_Tp>::min(Pointer<_Tp> *curNode, int type) {
  _Tp mindistance = 9999999;
  // the node to be returned
  Pointer<_Tp> *minNode = 0;
  if (type == 1) // the first quadrant
  {
    cout << "in minNode() now the loop is: " << endl;
    curNode->print();
    for (_Node *node = curNode->_next; node != curNode; node = node->_next) {
      node->print();
      if (node->_x >= curNode->_x && node->_y >= curNode->_y) {
        // horizontal conditions
        if (node->_next->_x < curNode->_x || node->_prev->_x < curNode->_x) {
          _Tp temp = node->_y - curNode->_y;
          if (temp < mindistance) {
            mindistance = temp;
            minNode = node;
          }
        }
        // vertical conditions
        if (node->_next->_y < curNode->_y || node->_prev->_y < curNode->_y) {
          _Tp temp = node->_x - curNode->_x;
          if (temp < mindistance) {
            mindistance = temp;
            minNode = node;
          }
        }
        // complex conditions
        // if in horizontal or vertical conditions
        // now the temp distance is larger
        _Tp temp = abs(node->_x - curNode->_x) + abs(node->_y - curNode->_y);
        if (temp < mindistance) {
          mindistance = temp;
          minNode = node;
        }
      }
    }
    return minNode;
  } else if (type == 2) // the second quadrant
  {
    cout << "in minNode() now the loop is: " << endl;
    curNode->print();
    for (_Node *node = curNode->_next; node != curNode; node = node->_next) {
      node->print();
      if (node->_x <= curNode->_x && node->_y >= curNode->_y) {
        if (node->_next->_x > curNode->_x || node->_prev->_x > curNode->_x) {
          _Tp temp = node->_y - curNode->_y;
          if (temp < mindistance) {
            mindistance = temp;
            minNode = node;
          }
        }
        if (node->_next->_y < curNode->_y || node->_prev->_y < curNode->_y) {
          _Tp temp = curNode->_x - node->_x;
          if (temp < mindistance) {
            mindistance = temp;
            minNode = node;
          }
        }
        _Tp temp = abs(node->_x - curNode->_x) + abs(node->_y - curNode->_y);
        if (temp < mindistance) {
          mindistance = temp;
          minNode = node;
        }
      }
    }
    return minNode;
  } else if (type == 3) // the third quadrant
  {
    cout << "in minNode() now the loop is: " << endl;
    curNode->print();
    for (_Node *node = curNode->_next; node != curNode; node = node->_next) {
      node->print();
      if (node->_x <= curNode->_x && node->_y <= curNode->_y) {
        if (node->_next->_x > curNode->_x || node->_prev->_x > curNode->_x) {
          _Tp temp = curNode->_y - node->_y;
          if (temp < mindistance) {
            mindistance = temp;
            minNode = node;
          }
        }
        if (node->_next->_y > curNode->_y || node->_prev->_y > curNode->_y) {
          _Tp temp = curNode->_x - node->_x;
          if (temp < mindistance) {
            mindistance = temp;
            minNode = node;
          }
        }
        _Tp temp = abs(node->_x - curNode->_x) + abs(node->_y - curNode->_y);
        if (temp < mindistance) {
          mindistance = temp;
          minNode = node;
        }
      }
    }
    return minNode;
  } else if (type == 4) // the fourth quadrant
  {
    cout << "in minNode() now the loop is: " << endl;
    curNode->print();
    for (_Node *node = curNode->_next; node != curNode; node = node->_next) {
      node->print();
      if (node->_x >= curNode->_x && node->_y <= curNode->_y) {
        if (node->_next->_x < curNode->_x || node->_prev->_x < curNode->_x) {
          _Tp temp = curNode->_y - node->_y;
          if (temp < mindistance) {
            mindistance = temp;
            minNode = node;
          }
        }
        if (node->_next->_y > curNode->_y || node->_prev->_y > curNode->_y) {
          _Tp temp = node->_x - curNode->_x;
          if (temp < mindistance) {
            mindistance = temp;
            minNode = node;
          }
        }
        _Tp temp = abs(node->_x - curNode->_x) + abs(node->_y - curNode->_y);
        if (temp < mindistance) {
          mindistance = temp;
          minNode = node;
        }
      }
    }
    return minNode;
  }
}

/** @cut at the position of the current node which
 *  corresponds to an angle of 270
 *  there are 28 small conditions in 4 big conditions
 *  types could be 1,2,3,4 which correspond to four
 *  conditions of the current node
 *  in some conditions new nodes may be created
 *  used in cut()
 *  below for the recursion version
 *  modify head and curNode as the head of the two got
 *  polygons after correctly cutting
 */
template <typename _Tp>
void Polygonc<_Tp>::createPolygon(Pointer<_Tp> *curNode, int type) {
  if (type == 1) // the first quadrant
  {
    // find the minNode first
    Pointer<_Tp> *minNode = min(curNode, 1);
    cout << "minNode is: ";
    minNode->print();
    // in each condition, we deal with the left part
    // or the upper part first
    // be aware that in vertical condition 1 and
    // horizontal condition 1 more conditions need to
    // be tested
    // in no-bevel polygons, prevvertical equals to
    // nexthorizontal and prevhorizontal equals to
    // nextvertical
    // vertical condition 1
    if (minNode->prevvertical() && minNode->_prev->_y < curNode->_y &&
        minNode->_y > curNode->_y &&
        ((minNode->_x - curNode->_x) <= (minNode->_y - curNode->_y) ||
         minNode->_next->_x > curNode->_x)) {
      cout << "type1:vertical condition 1" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      auto *newnode1 = new Pointer<_Tp>(0, 0, 0, 0);
      assert(newnode1 != 0 || !"memory allocation failure!");
      newnode1->_x = minNode->_x;
      newnode1->_y = curNode->_y;
      newnode1->prev(curNode->_prev);
      curNode->_prev->next(newnode1);
      newnode1->next(minNode);
      minNode->prev(newnode1);
      auto *newnode2 = new Pointer<_Tp>(newnode1);
      assert(newnode2 != 0 || !"memory allocation failure!");
      newnode2->next(curNode);
      curNode->prev(newnode2);
      newnode2->prev(minprev);
      minprev->next(newnode2);
      _addedPn.push_back(newnode1);
      _addedPn.push_back(newnode2);
      // head = newnode1;
      // curNode = newnode2;
      return;
    }
    // horizontal condition 1
    else if (minNode->nexthorizontal() && minNode->_next->_x < curNode->_x &&
             minNode->_x > curNode->_x &&
             ((minNode->_y - curNode->_y) <= (minNode->_x - curNode->_x) ||
              minNode->_prev->_y > curNode->_y)) {
      cout << "type1:horizontal condition 1" << endl;
      Pointer<_Tp> *curnext = curNode->_next;
      auto *newnode1 = new Pointer<_Tp>(0, 0, 0, 0);
      assert(newnode1 != 0 || !"memory allocation failure!");
      newnode1->_x = curNode->_x;
      newnode1->_y = minNode->_y;
      newnode1->next(minNode->_next);
      minNode->_next->prev(newnode1);
      newnode1->prev(curNode);
      curNode->next(newnode1);
      auto *newnode2 = new Pointer<_Tp>(newnode1);
      assert(newnode2 != 0 || !"memory allocation failure!");
      newnode2->next(curnext);
      curnext->prev(newnode2);
      newnode2->prev(minNode);
      minNode->next(newnode2);
      _addedPn.push_back(newnode1);
      _addedPn.push_back(newnode2);
      // head = newnode1;
      // curNode = newnode2;
      return;
    }
    // vertical condition 2
    else if (minNode->prevvertical() && minNode->_prev->_y < minNode->_y &&
             minNode->_y == curNode->_y) {
      cout << "type1:vertical condition 2" << endl;
      // Pointer<_Tp>* curprev = curNode->_prev;
      curNode->_prev->next(minNode->_next);
      minNode->_next->prev(curNode->_prev);
      curNode->prev(minNode);
      minNode->next(curNode);
      // head = curprev;
      // curNode doesn't need to change
      return;
    }
    // horizontal condition 2
    else if (minNode->nexthorizontal() && minNode->_next->_x < minNode->_x &&
             minNode->_x == curNode->_x) {
      cout << "type1:horizontal condition 2" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      Pointer<_Tp> *curnext = curNode->_next;
      curNode->next(minNode);
      minNode->prev(curNode);
      minprev->next(curnext);
      curnext->prev(minprev);
      // head = curNode;
      // curNode = minprev;
      return;
    }
    // complex condition 1
    // permit overlapping cutting
    else if (minNode->nextvertical() && minNode->_y < minNode->_next->_y &&
             minNode->_x < minNode->_prev->_x && minNode->_x > curNode->_x &&
             minNode->_y > curNode->_y) {
      cout << "type1:complex condition 1" << endl;
      Pointer<_Tp> *minnext = minNode->_next;
      Pointer<_Tp> *curnext = curNode->_next;
      minNode->_x = curnext->_x;
      minNode->next(curnext);
      curnext->prev(minNode);
      curNode->_x = minnext->_x;
      curNode->next(minnext);
      minnext->prev(curNode);
      return;
    }
    // complex condition 2
    // the condition that minNode == curNode
    // goes in complex condition 2
    else if (minNode->nextvertical() && minNode->_y < minNode->_next->_y &&
             minNode->_x < minNode->_prev->_x && minNode->_y == curNode->_y) {
      cout << "type1:complex condition 2" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      curNode->_prev->next(minNode);
      minNode->prev(curNode->_prev);
      minprev->next(curNode);
      curNode->prev(minprev);
      // head = minNode;
      // curNode doesn't need to change
      return;
    }
    // complex condition 3
    else if (minNode->nextvertical() && minNode->_y < minNode->_next->_y &&
             minNode->_x < minNode->_prev->_x && minNode->_x == curNode->_x) {
      cout << "type1:complex condition 3" << endl;
      Pointer<_Tp> *curnext = curNode->_next;
      curNode->next(minNode->_next);
      minNode->_next->prev(curNode);
      minNode->next(curnext);
      curnext->prev(minNode);
      // head = curNode;
      // curNode = minNode;
      return;
    }
  } else if (type == 2) // the second quadrant
  {
    Pointer<_Tp> *minNode = min(curNode, 2);
    cout << "minNode is: ";
    minNode->print();
    // vertical condition 1
    if (minNode->nextvertical() && minNode->_next->_y < curNode->_y &&
        minNode->_y > curNode->_y &&
        ((curNode->_x - minNode->_x) <= (minNode->_y - curNode->_y) ||
         minNode->_prev->_x < curNode->_x)) {
      cout << "type2:vertical condition 1" << endl;
      Pointer<_Tp> *minnext = minNode->_next;
      auto *newnode1 = new Pointer<_Tp>(0, 0, 0, 0);
      assert(newnode1 != 0 || !"memory allocation failure!");
      newnode1->_x = minNode->_x;
      newnode1->_y = curNode->_y;
      newnode1->next(curNode->_next);
      curNode->_next->prev(newnode1);
      newnode1->prev(minNode);
      minNode->next(newnode1);
      auto *newnode2 = new Pointer<_Tp>(newnode1);
      assert(newnode2 != 0 || !"memory allocation failure!");
      newnode2->next(minnext);
      minnext->prev(newnode2);
      newnode2->prev(curNode);
      curNode->next(newnode2);
      _addedPn.push_back(newnode1);
      _addedPn.push_back(newnode2);
      // head = newnode1;
      // curNode = newnode2;
      return;
    }
    // horizontal condition 1
    else if (minNode->prevhorizontal() && minNode->_prev->_x > curNode->_x &&
             minNode->_x < curNode->_x &&
             ((minNode->_y - curNode->_y) <= (curNode->_x - minNode->_x) ||
              minNode->_next->_y > curNode->_y)) {
      cout << "type2:horizontal condition 1" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      auto *newnode1 = new Pointer<_Tp>(0, 0, 0, 0);
      assert(newnode1 != 0 || !"memory allocation failure!");
      newnode1->_x = curNode->_x;
      newnode1->_y = minNode->_y;
      newnode1->next(minNode);
      minNode->prev(newnode1);
      newnode1->prev(curNode->_prev);
      curNode->_prev->next(newnode1);
      auto *newnode2 = new Pointer<_Tp>(newnode1);
      assert(newnode2 != 0 || !"memory allocation failure!");
      newnode2->next(curNode);
      curNode->prev(newnode2);
      newnode2->prev(minprev);
      minprev->next(newnode2);
      _addedPn.push_back(newnode1);
      _addedPn.push_back(newnode2);
      // head = newnode1;
      // curNode = newnode2;
      return;
    }
    // vertical condition 2
    else if (minNode->prevhorizontal() && minNode->_next->_y < minNode->_y &&
             minNode->_y == curNode->_y) {
      cout << "type2:vertical condition 2" << endl;
      // Pointer<_Tp>* minprev = minNode->_prev;
      minNode->_prev->next(curNode->_next);
      curNode->_next->prev(minNode->_prev);
      minNode->prev(curNode);
      curNode->next(minNode);
      // head = minprev;
      // curNode doesn't need to change
      return;
    }
    // horizontal condition 2
    else if (minNode->nexthorizontal() && minNode->_prev->_x > minNode->_x &&
             minNode->_x == curNode->_x) {
      cout << "type2:horizontal condition 2" << endl;
      Pointer<_Tp> *curprev = curNode->_prev;
      curprev->next(minNode->_next);
      minNode->_next->prev(curprev);
      minNode->next(curNode);
      curNode->prev(minNode);
      // head = curprev;
      // curNode doesn't need to change
      return;
    }
    // complex condition 1
    else if (minNode->nexthorizontal() && minNode->_y < minNode->_prev->_y &&
             minNode->_x > minNode->_next->_x && minNode->_x < curNode->_x &&
             minNode->_y > curNode->_y) {
      cout << "type2:complex condition 1" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      Pointer<_Tp> *curprev = curNode->_prev;
      minNode->_x = curprev->_x;
      minNode->prev(curprev);
      curprev->next(minNode);
      curNode->_x = minprev->_x;
      curNode->prev(minprev);
      minprev->next(curNode);
      return;
    }
    // complex condition 2
    else if (minNode->nexthorizontal() && minNode->_y < minNode->_prev->_y &&
             minNode->_x > minNode->_next->_x && minNode->_y == curNode->_y) {
      cout << "type2:complex condition 2" << endl;
      Pointer<_Tp> *curnext = curNode->_next;
      curNode->next(minNode->_next);
      minNode->_next->prev(curNode);
      minNode->next(curnext);
      curnext->prev(minNode);
      // head = curNode;
      // curNode = minNode;
      return;
    }
    // complex condition 3
    else if (minNode->nexthorizontal() && minNode->_y < minNode->_prev->_y &&
             minNode->_x > minNode->_next->_x && minNode->_x == curNode->_x) {
      cout << "type2:complex condition 3" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      curNode->_prev->next(minNode);
      minNode->prev(curNode->_prev);
      minprev->next(curNode);
      curNode->prev(minprev);
      // head = minNode;
      // curNode doesn't need to change
      return;
    }
  } else if (type == 3) // the third quadrant
  {
    Pointer<_Tp> *minNode = min(curNode, 3);
    cout << "minNode is: ";
    minNode->print();
    // vertical condition 1
    if (minNode->nexthorizontal() && minNode->_prev->_y > curNode->_y &&
        minNode->_y < curNode->_y &&
        ((curNode->_x - minNode->_x) <= (curNode->_y - minNode->_y) ||
         minNode->_next->_x < curNode->_x)) {
      cout << "type3:vertical condition 1" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      Pointer<_Tp> *curprev = curNode->_prev;
      auto *newnode1 = new Pointer<_Tp>(0, 0, 0, 0);
      assert(newnode1 != 0 || !"memory allocation failure!");
      newnode1->_x = minNode->_x;
      newnode1->_y = curNode->_y;
      newnode1->next(curNode);
      curNode->prev(newnode1);
      newnode1->prev(minprev);
      minprev->next(newnode1);
      auto *newnode2 = new Pointer<_Tp>(newnode1);
      assert(newnode2 != 0 || !"memory allocation failure!");
      newnode2->next(minNode);
      minNode->prev(newnode2);
      newnode2->prev(curprev);
      curprev->next(newnode2);
      _addedPn.push_back(newnode1);
      _addedPn.push_back(newnode2);
      // head = newnode1;
      // curNode = newnode2;
      return;
    }
    // horizontal condition 1
    else if (minNode->nexthorizontal() && minNode->_next->_x > curNode->_x &&
             minNode->_x < curNode->_x &&
             ((curNode->_y - minNode->_y) <= (curNode->_x - minNode->_x) ||
              minNode->_prev->_y < curNode->_y)) {
      cout << "type3:horizontal condition 1" << endl;
      Pointer<_Tp> *minnext = minNode->_next;
      auto *newnode1 = new Pointer<_Tp>(0, 0, 0, 0);
      assert(newnode1 != 0 || !"memory allocation failure!");
      newnode1->_x = curNode->_x;
      newnode1->_y = minNode->_y;
      newnode1->next(curNode->_next);
      curNode->_next->prev(newnode1);
      newnode1->prev(minNode);
      minNode->next(newnode1);
      auto *newnode2 = new Pointer<_Tp>(newnode1);
      assert(newnode2 != 0 || !"memory allocation failure!");
      newnode2->next(minnext);
      minnext->prev(newnode2);
      newnode2->prev(curNode);
      curNode->next(newnode2);
      _addedPn.push_back(newnode1);
      _addedPn.push_back(newnode2);
      // head = newnode1;
      // curNode = newnode2;
      return;
    }
    // vertical condition 2
    else if (minNode->prevvertical() && minNode->_prev->_y > minNode->_y &&
             minNode->_y == curNode->_y) {
      cout << "type3:vertical condition 2" << endl;
      Pointer<_Tp> *minnext = minNode->_next;
      Pointer<_Tp> *curprev = curNode->_prev;
      minNode->next(curNode);
      curNode->prev(minNode);
      curprev->next(minnext);
      minnext->prev(curprev);
      // head = minNode;
      // curNode = curprev;
      return;
    }
    // horizontal condition 2
    else if (minNode->nexthorizontal() && minNode->_next->_x > minNode->_x &&
             minNode->_x == curNode->_x) {
      cout << "type3:horizontal condition 2" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      minprev->next(curNode->_next);
      curNode->_next->prev(minprev);
      curNode->next(minNode);
      minNode->prev(curNode);
      // head = minprev;
      // curNode doesn't need to change
      return;
    }
    // complex condition 1
    else if (minNode->nextvertical() && minNode->_y > minNode->_next->_y &&
             minNode->_x > minNode->_prev->_x && minNode->_x < curNode->_x &&
             minNode->_y < curNode->_y) {
      cout << "type3:complex condition 1" << endl;
      Pointer<_Tp> *minnext = minNode->_next;
      Pointer<_Tp> *curnext = curNode->_next;
      curNode->_x = minnext->_x;
      curNode->next(minnext);
      minnext->prev(curNode);
      minNode->_x = curnext->_x;
      minNode->next(curnext);
      curnext->prev(minNode);
      return;
    }
    // complex condition 2
    else if (minNode->nextvertical() && minNode->_y > minNode->_next->_y &&
             minNode->_x > minNode->_prev->_x && minNode->_y == curNode->_y) {
      cout << "type3:complex condition 2" << endl;
      Pointer<_Tp> *curprev = curNode->_prev;
      minNode->_prev->next(curNode);
      curNode->prev(minNode->_prev);
      curprev->next(minNode);
      minNode->prev(curprev);
      // head = curNode;
      // curNode = minNode;
      return;
    }
    // complex condition 3
    else if (minNode->nextvertical() && minNode->_y > minNode->_next->_y &&
             minNode->_x > minNode->_prev->_x && minNode->_x == curNode->_x) {
      cout << "type3:complex condition 3" << endl;
      Pointer<_Tp> *minnext = minNode->_next;
      minNode->next(curNode->_next);
      curNode->_next->prev(minNode);
      curNode->next(minnext);
      minnext->prev(curNode);
      // head = minNode;
      // curNode doesn't need to change
      return;
    }
  } else if (type == 4) // the fourth quadrant
  {
    Pointer<_Tp> *minNode = min(curNode, 4);
    cout << "minNode is: ";
    minNode->print();
    // vertical condition 1
    if (minNode->nextvertical() && minNode->_next->_y > curNode->_y &&
        minNode->_y < curNode->_y &&
        ((minNode->_x - curNode->_x) <= (curNode->_y - minNode->_y) ||
         minNode->_prev->_x > curNode->_x)) {
      cout << "type4:vertical condition 1" << endl;
      Pointer<_Tp> *minnext = minNode->_next;
      auto *newnode1 = new Pointer<_Tp>(0, 0, 0, 0);
      assert(newnode1 != 0 || !"memory allocation failure!");
      newnode1->_x = minNode->_x;
      newnode1->_y = curNode->_y;
      newnode1->next(curNode->_next);
      curNode->_next->prev(newnode1);
      newnode1->prev(minNode);
      minNode->next(newnode1);
      auto *newnode2 = new Pointer<_Tp>(newnode1);
      assert(newnode2 != 0 || !"memory allocation failure!");
      newnode2->next(minnext);
      minnext->prev(newnode2);
      newnode2->prev(curNode);
      curNode->next(newnode2);
      _addedPn.push_back(newnode1);
      _addedPn.push_back(newnode2);
      // head = newnode1;
      // curNode = newnode2;
      return;
    }
    // horizontal condition 1
    else if (minNode->prevhorizontal() && minNode->_prev->_x < curNode->_x &&
             minNode->_x > curNode->_x &&
             ((minNode->_y - curNode->_y) <= (minNode->_x - curNode->_x) ||
              minNode->_next->_y < curNode->_y)) {
      cout << "type4:horizontal condition 1" << endl;
      Pointer<_Tp> *curprev = curNode->_prev;
      auto *newnode1 = new Pointer<_Tp>(0, 0, 0, 0);
      assert(newnode1 != 0 || !"memory allocation failure!");
      newnode1->_x = curNode->_x;
      newnode1->_y = minNode->_y;
      newnode1->next(curNode);
      curNode->prev(newnode1);
      newnode1->prev(minNode->_prev);
      minNode->_prev->next(newnode1);
      auto *newnode2 = new Pointer<_Tp>(newnode1);
      assert(newnode2 != 0 || !"memory allocation failure!");
      newnode2->next(minNode);
      minNode->prev(newnode2);
      newnode2->prev(curprev);
      curprev->next(newnode2);
      _addedPn.push_back(newnode1);
      _addedPn.push_back(newnode2);
      // head = newnode1;
      // curNode = newnode2;
      return;
    }
    // vertical condition 2
    else if (minNode->nextvertical() && minNode->_next->_y > minNode->_y &&
             minNode->_y == curNode->_y) {
      cout << "type4:vertical condition 2" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      Pointer<_Tp> *curnext = curNode->_next;
      curNode->next(minNode);
      minNode->prev(curNode);
      minprev->next(curnext);
      curnext->prev(minprev);
      // head = curNode;
      // curNode = minprev;
      return;
    }
    // horizontal condition 2
    else if (minNode->prevhorizontal() && minNode->_prev->_x < minNode->_x &&
             minNode->_x == curNode->_x) {
      cout << "type4:horizontal condition 2" << endl;
      Pointer<_Tp> *minnext = minNode->_next;
      Pointer<_Tp> *curprev = curNode->_prev;
      minNode->next(curNode);
      curNode->prev(minNode);
      curprev->next(minnext);
      minnext->prev(curprev);
      // head = minNode;
      // curNode = curprev;
      return;
    }
    // complex condition 1
    else if (minNode->prevvertical() && minNode->_y > minNode->_prev->_y &&
             minNode->_x < minNode->_next->_x && minNode->_x > curNode->_x &&
             minNode->_y < curNode->_y) {
      cout << "type4:complex condition 1" << endl;
      Pointer<_Tp> *minprev = minNode->_prev;
      Pointer<_Tp> *curprev = curNode->_prev;
      minNode->_x = curprev->_x;
      minNode->prev(curprev);
      curprev->next(minNode);
      curNode->_x = minprev->_x;
      curNode->prev(minprev);
      minprev->next(curNode);
      return;
    }
    // complex condition 2
    else if (minNode->prevvertical() && minNode->_y > minNode->_prev->_y &&
             minNode->_x < minNode->_next->_x && minNode->_y == curNode->_y) {
      cout << "type4:complex condition 2" << endl;
      Pointer<_Tp> *minnext = minNode->_next;
      minNode->next(curNode->_next);
      curNode->_next->prev(minNode);
      curNode->next(minnext);
      minnext->prev(curNode);
      // head = minNode;
      // curNode doesn't need to change
      return;
    }
    // complex condition 3
    else if (minNode->prevvertical() && minNode->_y > minNode->_prev->_y &&
             minNode->_x < minNode->_next->_x && minNode->_x == curNode->_x) {
      cout << "type4:complex condition 3" << endl;
      Pointer<_Tp> *curprev = curNode->_prev;
      minNode->_prev->next(curNode);
      curNode->prev(minNode->_prev);
      curprev->next(minNode);
      minNode->prev(curprev);
      // head = curNode;
      // curNode = minNode;
      return;
    }
  }
}

/** @the main function of cutting
 *  the got rectangles are put in _rectanglegroup[]
 *  assert that the polygon input is counterclockwise
 *  assert that the polygon input has no repeated points
 *  and points in the line of the prevenient point and the next point
 */
template <class _Tp> void Polygonc<_Tp>::cut() {
  // for each Pointer<int> in _Pn
  // deal with it if it corresponds to an angle of 270
  // new nodes created in createPolygon() couldn't have
  // angles of 270 and don't need to deal with
  size_t i;
  for (i = 0; i < _Pn.size(); i++) {
    Pointer<_Tp> *curNode = &_Pn[i];

    // type 1, the first quadrant
    if (curNode->prevhorizontal() && curNode->_x > curNode->_prev->_x &&
        curNode->_y > curNode->_next->_y) {
      cout << "type1:curNode is ";
      curNode->print();
      createPolygon(curNode, 1);
    }
    // type 2, the second quadrant
    else if (curNode->prevvertical() && curNode->_y > curNode->_prev->_y &&
             curNode->_x < curNode->_next->_x) {
      cout << "type2:curNode is ";
      curNode->print();
      createPolygon(curNode, 2);
    }
    // type 3, the third quadrant
    else if (curNode->prevhorizontal() && curNode->_x < curNode->_prev->_x &&
             curNode->_y < curNode->_next->_y) {
      cout << "type3:curNode is ";
      curNode->print();
      createPolygon(curNode, 3);
    }
    // type 4, the fourth quadrant
    else if (curNode->prevvertical() && curNode->_x > curNode->_next->_x &&
             curNode->_y < curNode->_prev->_y) {
      cout << "type4:curNode is ";
      curNode->print();
      createPolygon(curNode, 4);
    }
  }

  // since there are no angles of 270,
  // there are only rectangles left
  // now get them
  //_isRefered in pointer<_Tp> is used here
  for (i = 0; i < _Pn.size(); i++) {
    if (_Pn[i]._isRefered == false) {
      Pointer<_Tp> *curNode = &_Pn[i];

      if (curNode->_next->_next->_next->_next != curNode)
        cout << "error: there is a non-rectangle polygon left." << endl;

      cout << "a rectangle is got: " << endl;
      curNode->print();
      curNode->_isRefered = true;
      for (_Node *node = curNode->_next; node != curNode; node = node->_next) {
        node->print();
        node->_isRefered = true;
      }
      _rectanglegroup[_rectanglenum++] = curNode;
    }
  }

  // get rectangles whose points are all new nodes
  // created in createPolygon()
  for (i = 0; i < _addedPn.size(); i++) {
    if (_addedPn[i]->_isRefered == false) {
      Pointer<_Tp> *curNode = _addedPn[i];

      if (curNode->_next->_next->_next->_next != curNode)
        cout << "error: there is a non-rectangle polygon left." << endl;

      cout << "a rectangle is got: " << endl;
      curNode->print();
      curNode->_isRefered = true;
      for (_Node *node = curNode->_next; node != curNode; node = node->_next) {
        node->print();
        node->_isRefered = true;
      }
      _rectanglegroup[_rectanglenum++] = curNode;
    }
  }

  // part of recursion version which isn't in use now
  //   //recursion is over when the current polygon is a rectangle
  //   if(head->_next->_next->_next->_next == head)
  //     {
  //       cout << "a rectangle is got: " << endl;
  //       head->print();
  //       for(_Node* node = head->_next; node != head; node = node->_next)
  // 	node->print();
  //       _rectanglegroup[_rectanglenum++] = head;
  //       return;
  //     }

  //   //print the points of the current polygon
  //   cout << "points of the current polygon are:" << endl;
  //   head->print();
  //   for(_Node* node = head->_next; node != head; node = node->_next)
  //     node->print();

  //   //deal with head first
  //   //notice that each time createPolygon() is used
  //   //head and curNode will be changed!
  //   Pointer<_Tp>* curNode = head;
  //   if(curNode->prevhorizontal() && curNode->_x > curNode->_prev->_x
  //      && curNode->_y > curNode->_next->_y)
  //     {
  //       cout << "dealing with head, type is 1" << endl;
  //       cout << "head: ";
  //       head->print();
  //       cout << "curNode: ";
  //       curNode->print();
  //       createPolygon(head, curNode, 1);
  //       cout << "begin to cut the current head: ";
  //       head->print();
  //       cut(head);
  //       cout << "begin to cut the current curNode: ";
  //       curNode->print();
  //       cut(curNode);
  //       return;
  //     }
  //   else if(curNode->prevvertical() && curNode->_y > curNode->_prev->_y
  // 	  && curNode->_x < curNode->_next->_x)
  //     {
  //       cout << "dealing with head, type is 2" << endl;
  //       cout << "head: ";
  //       head->print();
  //       cout << "curNode: ";
  //       curNode->print();
  //       createPolygon(head, curNode, 2);
  //       cout << "begin to cut the current head: ";
  //       head->print();
  //       cut(head);
  //       cout << "begin to cut the current curNode: ";
  //       curNode->print();
  //       cut(curNode);
  //       return;
  //     }
  //   else if(curNode->prevhorizontal() && curNode->_x < curNode->_prev->_x
  // 	  && curNode->_y < curNode->_next->_y)
  //     {
  //       cout << "dealing with head, type is 3" << endl;
  //       cout << "head: ";
  //       head->print();
  //       cout << "curNode: ";
  //       curNode->print();
  //       createPolygon(head, curNode, 3);
  //       cout << "begin to cut the current head: ";
  //       head->print();
  //       cut(head);
  //       cout << "begin to cut the current curNode: ";
  //       curNode->print();
  //       cut(curNode);
  //       return;
  //     }
  //   else if(curNode->prevvertical() && curNode->_x > curNode->_next->_x
  // 	  && curNode->_y < curNode->_prev->_y)
  //     {
  //       cout << "dealing with head, type is 4" << endl;
  //       cout << "head: ";
  //       head->print();
  //       cout << "curNode: ";
  //       curNode->print();
  //       createPolygon(head, curNode, 4);
  //       cout << "begin to cut the current head: ";
  //       head->print();
  //       cut(head);
  //       cout << "begin to cut the current curNode: ";
  //       curNode->print();
  //       cut(curNode);
  //       return;
  //     }

  //   //deal with other nodes
  //   for(curNode = head->_next; curNode != head; curNode = curNode->_next)
  //     {
  //       if(curNode->prevhorizontal() && curNode->_x > curNode->_prev->_x
  // 	 && curNode->_y > curNode->_next->_y)
  // 	{
  // 	  cout << "dealing with other nodes, type is 1" << endl;
  // 	  cout << "head: ";
  // 	  head->print();
  // 	  cout << "curNode: ";
  // 	  curNode->print();
  // 	  createPolygon(head, curNode, 1);
  // 	  cout << "begin to cut the current head: ";
  // 	  head->print();
  // 	  cut(head);
  // 	  cout << "begin to cut the current curNode: ";
  // 	  curNode->print();
  // 	  cut(curNode);
  // 	  return;
  // 	}
  //       else if(curNode->prevvertical() && curNode->_y > curNode->_prev->_y
  // 	      && curNode->_x < curNode->_next->_x)
  // 	{
  // 	  cout << "dealing with other nodes, type is 2" << endl;
  // 	  cout << "head: ";
  // 	  head->print();
  // 	  cout << "curNode: ";
  // 	  curNode->print();
  // 	  createPolygon(head, curNode, 2);
  // 	  cout << "begin to cut the current head: ";
  // 	  head->print();
  // 	  cut(head);
  // 	  cout << "begin to cut the current curNode: ";
  // 	  curNode->print();
  // 	  cut(curNode);
  // 	  return;
  // 	}
  //       else if(curNode->prevhorizontal() && curNode->_x < curNode->_prev->_x
  // 	      && curNode->_y < curNode->_next->_y)
  // 	{
  // 	  cout << "dealing with other nodes, type is 3" << endl;
  // 	  cout << "head: ";
  // 	  head->print();
  // 	  cout << "curNode: ";
  // 	  curNode->print();
  // 	  createPolygon(head, curNode, 3);
  // 	  cout << "begin to cut the current head: ";
  // 	  head->print();
  // 	  cut(head);
  // 	  cout << "begin to cut the current curNode: ";
  // 	  curNode->print();
  // 	  cut(curNode);
  // 	  return;
  // 	}
  //       else if(curNode->prevvertical() && curNode->_x > curNode->_next->_x
  // 	      && curNode->_y < curNode->_prev->_y)
  // 	{
  // 	  cout << "dealing with other nodes, type is 4" << endl;
  // 	  cout << "head: ";
  // 	  head->print();
  // 	  cout << "curNode: ";
  // 	  curNode->print();
  // 	  createPolygon(head, curNode, 4);
  // 	  cout << "begin to cut the current head: ";
  // 	  head->print();
  // 	  cut(head);
  // 	  cout << "begin to cut the current curNode: ";
  // 	  curNode->print();
  // 	  cut(curNode);
  // 	  return;
  // 	}
  //     }
}

/** Explicit instantiation */
template class Polygonc<int>;
// template class Pointer<interval<int> >;
