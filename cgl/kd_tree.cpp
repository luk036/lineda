#include "kd_tree.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <rectangle.hpp>
#include <svgstream.hpp>

using std::vector;
using dsl::bt_node;
using recti::point;
using numeric::interval;

const int DELTA = 0;

// class svgstream;
//####namespace cgl {

// For nth_element()
inline bool SplitX(const rt_point &lhn, const rt_point &rhn) {
  // left hand node
  point lhs = lhn._coor;
  point rhs = rhn._coor;
  return (lhs.x() < rhs.x() || (lhs.x() == rhs.x() && lhs.y() < rhs.y()));
}

// For nth_element()
inline bool SplitY(const rt_point &lhn, const rt_point &rhn) {
  point lhs = lhn._coor;
  point rhs = rhn._coor;
  return (lhs.y() < rhs.y() || (lhs.y() == rhs.y() && lhs.x() < rhs.x()));
}

vector<rt_point> kd_tree::convert(bt_node<point> *root) {
  vector<rt_point> sinks;
  convert(root, sinks);
  return sinks;
}

void kd_tree::convert(bt_node<point> *root, vector<rt_point> &sinks) {
  rt_point temp;
  temp._coor = root->_data;
  temp._node = root;
  sinks.push_back(temp);
  if ((root->is_leaf())) {
    // _rn++;  //xxx
    return;
  }
  if (root->_left != nullptr) {
    convert(root->_left, sinks);
  }
  if (root->_right != nullptr) {
    convert(root->_right, sinks);
  }
  return;
}

//  Create a kd-tree from sinks.
kt_node<point> *kd_tree::create(vector<point> initial_sinks, Direction dir) {
  size_t size = initial_sinks.size();
  vector<rt_point> sinks(size);

  for (size_t i = 0; i < size; ++i) {
    sinks[i]._coor = initial_sinks[i];
  }
  // xxx  _rn = sinks.size(); //xxx

  const size_t num_nodes = 2 * sinks.size() - 1;
  _top = new kt_node<point>[num_nodes];
  _node_count = 0;
  _top = &build_kd_tree(sinks.begin(), sinks.end(), dir);
  assert(_node_count == num_nodes);
  assert(_top);
  return _top;
}

/** Create a topolopy given a routing-tree. */
kt_node<point> *kd_tree::create(bt_node<point> *routing_root, Direction dir) {
  vector<rt_point> sinks = convert(routing_root);
  _rn = sinks.size(); // xxx

  const size_t num_nodes = 2 * sinks.size() - 1;
  _top = new kt_node<point>[num_nodes];
  _node_count = 0;
  _top = &build_kd_tree(sinks.begin(), sinks.end(), dir);
  assert(_node_count == num_nodes);
  assert(_top);
  return _top;
}

/** The algorithm is similar to the construction of kd-tree.
    Reference: Compuational Geometry: Algorithms and Applications,
    M. de Berg et al., Springer Verlag 1997.
    Time complexity: O(N*logN) Storage requirement: O(N)
    where N is the number of sinks
*/
kt_node<point> &kd_tree::build_kd_tree(const sink_it &pos1, const sink_it &pos2,
                                       Direction dir) {
  kt_node<point> &v = _top[_node_count++];

  if (pos2 - pos1 == 1) { // P contains only one point
    v._data = pos1->_coor;
    v._node = pos1->_node;
    return v; // return a leaf storing this point
  }

  const auto pos = partition(pos1, pos2, dir);

  // Parent node stores the splitting line.
  // if a vertical line, store x coordinate only.
  // else,y only.
  v._data = pos->_coor;
  v._node = pos->_node;
  // point tmp;
  if (dir == 1) { // split at x,
    v._data = point(v._data.x(), INFINITE);
    // v._data = tmp; //stores x.
  } else {
    v._data = point(INFINITE, v._data.y());
  }
  // xxx  if (dir == 1) { // split at x,
  // xxx    v._data._y = INFINITE; //stores x.
  // xxx  } else {
  // xxx    v._data._x = INFINITE;
  // xxx  }
  // xxx
  // pos+1: the leaf on the splitting line belongs to the left subtree.
  v._left = &build_kd_tree(pos1, pos + 1, dir);
  v._right = &build_kd_tree(pos + 1, pos2, dir);
  v._left->_parent = v._right->_parent = &v;
  return v;
}

vector<rt_point>::iterator
kd_tree::partition(const sink_it &pos1, const sink_it &pos2, Direction &dir) {
  // Get the [n/2]th position. n:number of the leaves to be sorted.
  const auto median_pos = pos1 + (pos2 - 1 - pos1) / 2;

  if (dir == VERTICAL) { // vertical direction
    // Split P into two subsets with a vertical line l through the
    // median x-coordinate of the points in P. Let P1 be the set of
    // points to the left and P2 be the set of points to the
    // right. Points exactly on the line belong to *P2*
    std::nth_element(pos1, median_pos, pos2, SplitX);
    dir = HORIZONTAL;
  } else {
    // Split P into two subsets with a horizontal line l through the
    // median y-coordinate of the points in P. Let P1 be the set of
    // points above I and P2 be the set of points below I.
    // Points exactly on the line belong to *P2*
    std::nth_element(pos1, median_pos, pos2, SplitY);
    dir = VERTICAL;
  }
  return median_pos;
}

/** Select points, searching from the root. */
void kd_tree::select_point(point &s) { select_point(_top, s); }

void kd_tree::select_rect(kt_rectangle &rect) { select_rect(_top, rect); }

void kd_tree::select_rect(point top_left, point bottom_right) {
  select_rect(_top, top_left, bottom_right);
}

// the following "select" : searching from the given node of kd-tree.
/** Select the given point . */
void kd_tree::select_point(kt_node<point> *top, point &s) {
  int x = s.x();
  int y = s.y();

  kt_rectangle range(x, y, x, y);
  select_rect(top, range);
}

// xxx/** Select a point by a given range . */
// xxxvoid kd_tree::select_point(kt_node<point>* top, point top_left, point
// bottom_right)
// xxx{
// xxx  kt_rectangle rect(top_left, bottom_right);
// xxx  select_rect(top, rect);
// xxx}
// xxx

/** Select points given a rectangle range. */
void kd_tree::select_rect(kt_node<point> *top, kt_rectangle &r) {
  // Get the valid rectangular range.
  if (r.is_valid()) {
    // xxx    r._top_left = point(r._top_left.x() - DELTA, r._top_left.y() -
    // DELTA);      // adjust
    // xxx    r._bottom_right = point(r._bottom_right.x() + DELTA,
    // r._bottom_right.y() + DELTA);
    // xxx r.print_rect();
    // xxx cout<<"routing number: "<<_rn;
    // cout <<"into kd_tree\t";
    if (_selected_size != 0) {
      _selected_size = 0;
      _points_selected.clear();
      _sinks_selected.clear();
    }
    search_kd_tree(top, r);
  }
}

/** Select points given two points on the cross of the range. */
void kd_tree::select_rect(kt_node<point> *top, point top_left,
                          point bottom_right) {
  kt_rectangle rect(top_left, bottom_right);
  select_rect(top, rect);
  //  if(_selected_size > 1) {
  // select_point(top, _points_selected[0]->_data);
  //}
}

/** Searching-- Input:the root of a (subtree of a) kd-tree and a range R.*/
void kd_tree::search_kd_tree(kt_node<point> *v, kt_rectangle &range) {
  int x1 = range._top_left.x();
  int x2 = range._bottom_right.x();
  int y1 = range._top_left.y();
  int y2 = range._bottom_right.y();
  int x = v->_data.x();
  int y = v->_data.y();

  // if v is a leaf, report the point stored at v if it lies in R
  if (v->is_leaf()) { // is a leaf : (v._left == 0 && v._right ==0)
    if ((x >= x1) && (y >= y1) && (x <= x2) && (y <= y2)) { // lies in R
      report_subtree(v);
    }
    return;
  }

  kt_rectangle region = get_region(v);
  kt_rectangle left_region = get_region(v->_left, region);
  kt_rectangle right_region = get_region(v->_right, region);

  // if region(lc(v)) is fully contained in R then ReportSubtree(lc(v))
  if (left_region.is_fully_contained(range)) {
    report_subtree(v->_left);
  }
  // else if region(lc(v)) intersects R then  SearchKdTree(lc(v),R)
  else if (left_region.is_overlap(
               range)) { // else if  region(lc(v)) intersects R
    search_kd_tree(v->_left, left_region, range); // then  SearchKdTree(lc(v),R)
  }

  // if region(rc(v)) is fully contained in R then ReportSubtree(rc(v))
  if (right_region.is_fully_contained(range)) {
    report_subtree(v->_right);
  }
  // else if region(rc(v)) intersects R then  SearchKdTree(rc(v),R)
  else if (right_region.is_overlap(range)) {
    search_kd_tree(v->_right, right_region, range);
  }
}

/** Searching-- Input:the root--v of a subtree of a kd-tree, the parent-region
 * of v and a range R.*/
void kd_tree::search_kd_tree(
    kt_node<point> *v, kt_rectangle &region,
    kt_rectangle &range) // pre_range: the exact coordinates.
{
  int x1 = range._top_left.x();
  int x2 = range._bottom_right.x();
  int y1 = range._top_left.y();
  int y2 = range._bottom_right.y();
  int x = v->_data.x();
  int y = v->_data.y();

  // if v is a leaf, Report the point stored at v if it lies in R.
  if (v->is_leaf()) { // is a leaf : (v._left == 0 && v._right ==0)
    if ((x >= x1) && (y >= y1) && (x <= x2) && (y <= y2)) { // lies in R
      report_subtree(v);
    }
    return;
  }

  kt_rectangle left_region = get_region(v->_left, region);

  // if region(lc(v)) is fully contained in R then ReportSubtree(lc(v))
  if (left_region.is_fully_contained(range)) {
    report_subtree(v->_left);
  }
  // else if region(lc(v)) intersects R then  SearchKdTree(lc(v),R)
  else if (left_region.is_overlap(
               range)) { // else if  region(lc(v)) intersects R
    search_kd_tree(v->_left, left_region, range); // then  SearchKdTree(lc(v),R)
  }

  kt_rectangle right_region = get_region(v->_right, region);
  // if region(rc(v)) is fully contained in R then ReportSubtree(rc(v))
  if (right_region.is_fully_contained(range)) {
    report_subtree(v->_right);
  }
  // else if region(rc(v)) intersects R then  SearchKdTree(rc(v),R)
  else if (right_region.is_overlap(range)) {
    search_kd_tree(v->_right, right_region, range);
  }
}

/** Get the corresponding region of v.
    input:node v--the root of a (subtree of a) kd-tree. */
kt_rectangle kd_tree::get_region(kt_node<point> *v) {
  kt_rectangle region;
  kt_rectangle parent_region = region;

  if (v->is_root()) { // root--the whole plane.
    return region;
  }

  if (v->is_leaf()) {
    region._top_left = v->_data;
    region._bottom_right = v->_data;
    return region; // leaf--itself.
  }

  parent_region = get_region(v->_parent);
  region = parent_region;

  // Get the intersection of parent_region and the region defined by the
  // splitting line stored in v.
  point data = v->_parent->_data;
  if (data.y() == INFINITE) {     // Split at x, change x.
    if (v == v->_parent->_left) { // left child: the left plane
      // xxx region._bottom_right._x = data.x();
      region._bottom_right = point(data.x(), region._bottom_right.y());
    } else { // right child: the right plane
      // xxx region._top_left._x = data.x();
      region._top_left = point(data.x(), region._top_left.y());
    }
  } else if (data.x() == INFINITE) { // Split at y.
    if (v == v->_parent->_right) {   // right child: the plane below L.
      // xxx region._top_left._y = data.y();
      region._top_left = point(region._top_left.x(), data.y());
    } else { // left child: the plane above.
      // xxx region._bottom_right._y = data.y();
      region._bottom_right = point(region._bottom_right.x(), data.y());
    }
  }
  return region;
}

/** Get the corresponding region of v.
    input: 1.node v--the root of a (subtree of a) kd-tree;
           2.the corresponding region of the parent of v.
*/
kt_rectangle kd_tree::get_region(kt_node<point> *v,
                                 kt_rectangle parent_region) {
  kt_rectangle region;
  region = parent_region;

  if (v->is_leaf()) {
    region._top_left = v->_data;
    region._bottom_right = v->_data;
    return region;
  }

  // Get the intersection between parent_region and the region defined by the
  // splitting line stored in v.
  point data = v->_parent->_data;
  if (data.y() == INFINITE) {     // Split at x, change x.
    if (v == v->_parent->_left) { // left child: the left plane
      // xxx region._bottom_right._x = data.x();
      region._bottom_right = point(data.x(), region._bottom_right.y());
    } else { // right child: the right plane
      // xxx region._top_left._x = data.x();
      region._top_left = point(data.x(), region._top_left.y());
    }
  } else if (data.x() == INFINITE) { // Split at y.
    if (v == v->_parent->_right) {   // right child: the plane below L.
      // xxx region._top_left._y = data.y();
      region._top_left = point(region._top_left.x(), data.y());
    } else { // left child: the plane above.
      // xxx region._bottom_right._y = data.y();
      region._bottom_right = point(region._bottom_right.x(), data.y());
    }
  }
  return region;
}

void kd_tree::report_subtree(kt_node<point> *v) {
  if (v->is_leaf()) {
    // xxxcout<<"data: (" << v->_node->_data.x() << ", " << v->_node->_data.y()
    // << ")" << endl;
    _points_selected.push_back(v->_node);
    _sinks_selected.push_back(v->_data);

    //  cout<<"point-selected's data: "<<_selected_size<<": "<<
    //  _points_selected[_selected_size]->_data.x()<<", "<<
    //  _points_selected[_selected_size]->_data.y()<< endl;
    ++_selected_size;
    // xxxcout<< "selected-size: "<< _selected_size<< endl;

    // xxxcout<<"leaf: (" << v->_data.x() << ", " << v->_data.y() << ")" <<
    // endl;
  } else {
    // xxxcout<<"node:: (" << v->_data.x() << ", " << v->_data.y() << ")" <<
    // endl;
    report_subtree(v->_left);
    report_subtree(v->_right);
  }
}

// The functions below just for testing.      //xxxxxxx

void kd_tree::report_subtree1(kt_node<point> *v) {
  if (v->is_leaf()) {
    cout << "point: (" << v->_data.x() << ", " << v->_data.y() << "), ";
    //   cout<<"leaf: (" << v->_data.x() << ", " << v->_data.y() << ")" << endl;
  } else {
    // cout<<"node:: (" << v->_data.x() << ", " << v->_data.y() << ")" << endl;
    report_subtree1(v->_left);
    report_subtree1(v->_right);
  }
}

void kd_tree::print_selection(std::vector<kt_node<point> *> points) {
  int i = 0;
  for (auto it = points.begin(); it != points.end(); ++it) {
    cout << "points' data: " << i << ": " << points[i]->_data.x() << ", "
         << points[i]->_data.y() << endl;
    ++i;
  }
}

// Create a topolopy given a routing-tree.
kt_node<point> *kd_tree::kd_create(kt_node<point> *kt_root, Direction dir) {
  vector<rt_point> sinks = kd_convert(kt_root);

  const size_t num_nodes = 2 * sinks.size() - 1;
  _top = new kt_node<point>[num_nodes];
  _node_count = 0;
  _top = &build_kd_tree(sinks.begin(), sinks.end(), dir);
  assert(_node_count == num_nodes);
  assert(_top);
  return _top;
}

vector<rt_point> kd_tree::kd_convert(kt_node<point> *root) {
  vector<rt_point> sinks;
  kd_convert(root, sinks);
  return sinks;
}

void kd_tree::kd_convert(kt_node<point> *root, vector<rt_point> &sinks) {
  rt_point temp;

  if ((root->is_leaf())) {
    temp._coor = root->_data;
    temp._node = (bt_node<point> *)root;
    // xxxcout<<"node data:(" << temp._node->_data.x() << ", " <<
    // temp._node->_data.y() << ")" << endl;  //xxx
    sinks.push_back(temp);
    return;
  }
  if (root->_left != nullptr) {
    kd_convert(root->_left, sinks);
  }
  if (root->_right != nullptr) {
    kd_convert(root->_right, sinks);
  }
  return;
}

/** Print out the tree in .svg format */
svgstream &operator<<(svgstream &os, const kd_tree &cs) {
  rectangle bound(interval<int>(START, INFINITE),
                  interval<int>(START, INFINITE));
  cs.print_subtree(os, cs._top, kd_tree::VERTICAL, bound);
  return os;
}

// xxx void kd_tree::print_subtree(svgstream& os, const kt_node<point>* v,
// xxx                             Direction dir, rectangle R) const
// xxx {
// xxx   if (v->is_leaf()) {
// xxx     os <<< v->_data << "\n";
// xxx     return;
// xxx   }
// xxx
// xxx   rectangle R1;
// xxx   rectangle R2;
// xxx   const interval<int> xb = R.x_bound();
// xxx   const interval<int> yb = R.y_bound();
// xxx
// xxx   if (dir == VERTICAL) {
// xxx     // Draw vertical line
// xxx     const int x = v->_data.x();
// xxx     os << "<line"
// xxx        << " x1=\"" << x << "\""
// xxx        << " y1=\"" << yb.lower() << "\""
// xxx        << " x2=\"" << x << "\""
// xxx        << " y2=\"" << yb.upper() << "\""
// xxx        << " style=\"fill:green;stroke:green\""
// xxx        << "/>\n";
// xxx     R1 = rectangle(interval<int>(xb.lower(), x), yb);
// xxx     R2 = rectangle(interval<int>(x, xb.upper()), yb);
// xxx     dir = HORIZONTAL;
// xxx   }
// xxx   else {
// xxx     // Draw horizontal line
// xxx     const int y = v->_data.y();
// xxx     os << "<line"
// xxx        << " x1=\"" << xb.lower() << "\""
// xxx        << " y1=\"" << y << "\""
// xxx        << " x2=\"" << xb.upper() << "\""
// xxx        << " y2=\"" << y << "\""
// xxx        << " style=\"fill:blue;stroke:blue\""
// xxx        << "/>\n";
// xxx     R1 = rectangle(xb, interval<int>(yb.lower(), y));
// xxx     R2 = rectangle(xb, interval<int>(y, yb.upper()));
// xxx     dir = VERTICAL;
// xxx   }
// xxx
// xxx   print_subtree(os, v->_left, dir, R1);
// xxx   print_subtree(os, v->_right, dir, R2);
// xxx }

//####}//namespace
