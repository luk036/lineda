#include "polygonn.hpp"
#include "pointer.hpp"
#include "rectlink.hpp"

#include <algorithm>
#include <iostream>

using namespace std;

namespace recti {

template <typename _Tp>
Polygonn<_Tp>::Polygonn(Pointer<_Tp> *head, int headnum, int rectanglenum)
    : _head(head), _pointernum(headnum), _rectanglenum(rectanglenum) {}

// ??? no destructor

// find the most left-down point.
template <typename _Tp>
Pointer<_Tp> *Polygonn<_Tp>::leftdown(Pointer<_Tp> *head) {
  _Node *temp = head;

  for (_Node *next = head->_next; next != head; next = next->_next) {
    if (temp->_x > next->_x || (temp->_x == next->_x && temp->_y > next->_y)) {
      temp = next;
    }
  }

  return temp;
}

// normalize the polygon and set the NO. of the points in the polygon.
template <typename _Tp>
Pointer<_Tp> *Polygonn<_Tp>::normalize(Pointer<_Tp> *head) {
  head = Polygonn<_Tp>::leftdown(head);

  if (head->_prev->_x != head->_x) { // not anticlockwise
    reverse(head);
    head = Polygonn<_Tp>::leftdown(head);
  }
  assert(head->_prev->_x == head->_x);

  return head;
}

template <typename _Tp> void reverse(Pointer<_Tp> *head) {
  Pointer<_Tp> *curr = head;
  std::swap(curr->_next, curr->_prev);
  curr = curr->_next;
  for (; curr != head; curr = curr->_next) {
    std::swap(curr->_next, curr->_prev);
  }
}

// creat newnode when cut the polygon.in this function only determine
// the x and y coordinate of the newnode.
template <typename _Tp>
Pointer<_Tp> *draw(const Pointer<_Tp> *xnode, const Pointer<_Tp> *ynode) {
  auto *newnode = new Pointer<_Tp>(0, 0, nullptr, nullptr);
  assert(newnode != nullptr || !"memory allocation failure!");
  newnode->_x = xnode->_x;
  newnode->_y = ynode->_y;
  return newnode;
}

// find whether there exists a inside node or not,and find the most
// left-down inside node.
template <typename _Tp>
Pointer<_Tp> *search(Pointer<_Tp> *thirdnode, Pointer<_Tp> *firstnode,
                     Pointer<_Tp> *behind) {
  Pointer<_Tp> *temp = nullptr;

  _Tp thirdx = thirdnode->_x;
  _Tp thirdy = thirdnode->_y;
  _Tp firstx = firstnode->_x;
  _Tp firsty = firstnode->_y;

  for (; behind != firstnode; behind = behind->_prev) {
    if (behind->_x != behind->_next->_x)
      continue;
    if (behind->_x <= thirdx && behind->_x > firstx && behind->_y > firsty &&
        behind->_y < thirdy) {
      if (temp == nullptr || temp->_x > behind->_x ||
          (temp->_x == behind->_x && temp->_y > behind->_y))
        temp = behind;
    }
  }

  return temp;
}

/** creat two new polygons(or rectangle) when cut the
    polygon.type11:no inside node and cut the polygon downwards from
    thirdnode(clock-wise);type21:exist inside node(inside node is the
    last node) and cut the polygon upwards from the inside
    node;type22:exist inside node and cut the polygon leftwards from the
    inside node. */
template <typename _Tp>
void creatpolygon(int type, Pointer<_Tp> *node, Pointer<_Tp> *firstnode,
                  Pointer<_Tp> *secondnode, Pointer<_Tp> *thirdnode,
                  Pointer<_Tp> *centernode) {
  Pointer<_Tp> *fourthnode = thirdnode->_prev;
  Pointer<_Tp> *lastnode = firstnode->_next;
  Pointer<_Tp> *follownode;

  if (type == 110) // node created by draw program(cut down) and the
  // fourthnode is below or above the thirdnode.
  {
    Pointer<_Tp> *lastnode2 = lastnode->_next;
    lastnode2->_prev = fourthnode;
    fourthnode->_next = lastnode2;
    lastnode->_next = thirdnode;
    thirdnode->_prev = lastnode;
    return;
  }

  auto *copynode = new Pointer<_Tp>(node->_x, node->_y, nullptr, nullptr);

  if (type == 111) // node created by draw program(cut down) and the
  // fourthnode is below or above the thirdnode.
  {
    node->_prev = fourthnode;
    fourthnode->_next = node;
    node->_next = lastnode;
    lastnode->_prev = node;

    copynode->_prev = firstnode;
    firstnode->_next = copynode;
    copynode->_next = thirdnode;
    thirdnode->_prev = copynode;
  }

  else if (type == 112) // node created by draw program(cut up) and the
  // fourthnode is below or above the thirdnode.
  {
    node->_prev = thirdnode;
    thirdnode->_next = node;
    Pointer<_Tp> *lastnode2 = lastnode->_next;
    node->_next = lastnode2;
    lastnode2->_prev = node;

    copynode->_next = secondnode;
    secondnode->_prev = copynode;
    copynode->_prev = lastnode;
    lastnode->_next = copynode;
  }

  else if (type == 22) // node created by draw program(cut left) due to
  // the center node and the follownode is right the
  // centernode.
  {
    Pointer<_Tp> *prevcenternode = centernode->_next;
    Pointer<_Tp> *nextfirstnode = firstnode->_next;
    follownode = firstnode->_next;

    // cout<<"TTT"<<firstnode->_num<<"\n";
    copynode->_prev = firstnode;
    firstnode->_next = copynode;
    copynode->_next = prevcenternode;
    prevcenternode->_prev = copynode;

    node->_prev = centernode;
    centernode->_next = node;
    node->_next = nextfirstnode;
    nextfirstnode->_prev = node;
  }
}

template <typename _Tp> void Polygonn<_Tp>::cut(Pointer<_Tp> *head) {
  // cout<<"step1:normalize"<<endl;
  head = normalize(head);
  // cout<<"step2:search"<<endl;
  _Tp A1 = area(head);

  Pointer<_Tp> *firstnode = head;
  Pointer<_Tp> *secondnode = head->_prev;
  Pointer<_Tp> *thirdnode = secondnode->_prev;

  if (firstnode->_next == thirdnode->_prev) { // rectangle
    _rectanglegroup[_rectanglenum++] = leftdown(head);
    return;
  }

  Pointer<_Tp> aNode(0, 0, nullptr, nullptr);
  aNode._y = thirdnode->_y;
  aNode._x = std::min(thirdnode->_x, firstnode->_next->_x);
  Pointer<_Tp> *centernode = search(&aNode, firstnode, thirdnode->_prev);

  // cout<<"step3:sort"<<endl;
  if (centernode == nullptr) // no inside node type 11.
  {
    // cout<<"type11"<<endl;
    // cout<<"1:draw"<<endl;
    Pointer<_Tp> *newnode = nullptr;
    if (firstnode->_next->_x > thirdnode->_x) {
      newnode = draw(thirdnode, firstnode);
      // cout<<"2:creatpolygon"<<endl;
      creatpolygon(111, newnode, firstnode, secondnode, thirdnode, centernode);
    } else if (firstnode->_next->_x < thirdnode->_x) {
      newnode = draw(firstnode->_next, thirdnode);
      // cout<<"2:creatpolygon"<<endl;
      creatpolygon(112, newnode, firstnode, secondnode, thirdnode, centernode);
    } else {
      newnode = firstnode->_next->_next;
      creatpolygon(110, newnode, firstnode, secondnode, thirdnode, centernode);
    }
    // cout<<"3:normalize"<<endl;
    firstnode = normalize(firstnode);
    _rectanglegroup[_rectanglenum++] = leftdown(firstnode);
    // xxx newnode = normalize(newnode);
    assert(area(firstnode) + area(newnode) == A1);
    cut(newnode);
  } else // exist inside node.
  {
    Pointer<_Tp> *newnode;

    newnode = draw(centernode, firstnode);

    // cout<<"2:creatpolygon"<<endl;
    creatpolygon(22, newnode, firstnode, secondnode, thirdnode, centernode);

    // cout<<"3:normalize"<<endl;
    // xxx newnode = normalize(newnode);
    // xxx firstnode = normalize(firstnode);
    assert(area(firstnode) + area(newnode) == A1);

    cut(firstnode);
    cut(newnode);
  }
}

template <typename _Tp> void Polygonn<_Tp>::enlarge(_Tp index) {
  _Tp width, height;
  Pointer<_Tp> *node, *nextnode, *prevnode;

  if (_rectanglenum < 1)
    // cout << "no rectangle."<<endl;

    for (int i = 0; i < _rectanglenum; i++) {
      node = _rectanglegroup[i];
      nextnode = node->_next;
      prevnode = node->_prev;

      width = nextnode->_x - node->_x;
      height = prevnode->_y - node->_y;
      // cout<<"width="<<width<<","<<"height="<<height<<endl;

      _rectanglegroup[i]->_x -= width * (index - 1) / 2;
      _rectanglegroup[i]->_y -= height * (index - 1) / 2;

      // cout<<"x="<< _rectanglegroup[i]->_x<<","<<"y="<<
      // _rectanglegroup[i]->_y<<endl;

      Rectlink<_Tp> rec(*_rectanglegroup[i], index * width, index * height);
      _rectanglestack[i] = &rec;

      // cout<<"rectangle:"<<"("<<rec.width()<<","<<rec.height()<<")"<<endl;
      // cout<<"ll"<<"("<<rec.llx()<<","<<rec.lly()<<")"<<"
      // "<<"ur"<<"("<<rec.urx()<<","<<rec.ury()<<")"<<endl;
    }
}

// caculate the area of the polygon.
template <typename _Tp> _Tp Polygonn<_Tp>::area(Pointer<_Tp> *head) {
  // head = normalize(head);
  _Tp dresult = 0, result = 0;
  Pointer<_Tp> *curr = head;
  Pointer<_Tp> *prev = curr->_prev;
  dresult = (curr->_x * prev->_y) - (curr->_y * prev->_x);
  curr = prev;
  prev = prev->_prev;
  // xxx for(int i=0 ;i < head->_pointernum; i ++)
  for (; curr != head; curr = prev, prev = prev->_prev) {
    dresult = dresult + ((curr->_x * prev->_y) - (curr->_y * prev->_x));
    // xxx curr=prev;
    // xxx prev=prev->_prev;
  }
  if (dresult < 0)
    dresult = -dresult;

  // cout<<"dresult="<<dresult<<endl;
  result = dresult / 2;
  // cout<<"result="<<result<<endl;
  return result;
}

// compare the sum of area of rectangles with the area of polygon.
template <typename _Tp> void Polygonn<_Tp>::cparea(_Tp area) {
  _Tp width, height, recarea = 0;
  Pointer<_Tp> *node, *nextnode, *prevnode;

  // if(_rectanglenum<1)
  //  cout << "no rectangle."<<endl;

  for (int i = 0; i < _rectanglenum; i++) {
    node = _rectanglegroup[i];
    nextnode = node->_next;
    prevnode = node->_prev;

    width = nextnode->_x - node->_x;
    height = prevnode->_y - node->_y;

    Rectlink<_Tp> rec(*_rectanglegroup[i], width, height);
    _rectanglestack[i] = &rec;
    recarea = recarea + rec.area();
  }
  cout << "recarea=" << recarea << endl;

  if (recarea == area)
    cout << "right" << endl;
  else
    cout << "wrong" << endl;
}

/** Explicit instantiation */
template class Polygonn<int>;
}
