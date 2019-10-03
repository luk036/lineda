#ifndef KD_TREE_HPP
#define KD_TREE_HPP

#include "kt_rectangle.hpp"
#include <iosfwd> // for declaration of std::istream and std::ostream
#include <rectangle.hpp>
#include <svgstream.hpp>
#include <vector>

#ifndef DSL_BT_NODE_HPP
#include <bt_node.hpp>
#endif
#ifndef RECTI_POINT_HPP
#include <point.hpp>
#endif
#include <rectifwd.hpp>

// using std::vector;

// template <typename _Tp> class kt_node;  //necessary??-test
// class rt_point;
class svgstream;

//####namespace cgl{//typedef include or outside??
/** ??? */
class rt_point // Define sinks converting from a routing tree.
{
public:
  recti::point _coor;                // Coordinates
  dsl::bt_node<recti::point> *_node; // Pointer _node -> node in routing tree

  rt_point() : _node(nullptr){};
};

/** Node for kd-tree */
template <typename _Tp>
class kt_node // Define nodes in kd-tree.
{
public:
  kt_node<_Tp> *_parent;    /**< pointer to parent node */
  kt_node<_Tp> *_left;      /**< pointer to left node */
  kt_node<_Tp> *_right;     /**< pointer to right node */
  _Tp _data;                /**< data that this node carries */
  dsl::bt_node<_Tp> *_node; /**< pointer to the original node. */

  /** Default constructor */
  inline kt_node()
      : _parent(nullptr), _left(nullptr), _right(nullptr), _data(),
        _node(nullptr) {}

  /** Copy constructor */
  kt_node(const kt_node<_Tp> &copy)
      : _parent(copy._parent), _left(copy._left), _right(copy._right),
        _data(copy._data) {}

  /** Assignment */
  kt_node<_Tp> &operator=(const kt_node<_Tp> &rhs) {
    _parent = rhs._parent;
    _left = rhs._left;
    _right = rhs._right;
    _data = rhs._data;
    return *this;
  }

  /** @return whether this node is a root. */
  inline bool is_root() const { return _parent == nullptr; }

  /** @return whether this node is leaf */
  inline bool is_leaf() const { return _left == nullptr && _right == nullptr; }

  /** Equality */
  inline bool operator==(const kt_node<_Tp> &rhs) const {
    if (this == &rhs)
      return true;
    return _parent == rhs._parent && _data == rhs._data && _left == rhs._left &&
           _right == rhs._right;
  }
};

typedef std::vector<rt_point>::iterator sink_it;

/** kd tree */
class kd_tree {
  friend svgstream &operator<<(svgstream &os, const kd_tree &cs);

public:
  enum Direction { VERTICAL, HORIZONTAL };
  size_t _node_count;          // The total number of nodes.
  kt_node<recti::point> *_top; // The root of kd-tree
  std::vector<dsl::bt_node<recti::point> *> _points_selected; // pointers of
                                                              // points found by
                                                              // searching a
                                                              // rectangular
                                                              // range.
  std::vector<recti::point>
      _sinks_selected;   // for searching of a kd-tree created from sinks.
  size_t _selected_size; // number of points found by searching.

  /** Constructor */
  kd_tree() : _node_count(0), _top(nullptr), _selected_size(0){};
  kd_tree(std::vector<recti::point> &sinks);
  kd_tree(const kd_tree &tree1)
      : _node_count(tree1._node_count), _selected_size(0) {
    // _node_count = tree1._node_count;
    _top = new kt_node<recti::point>[_node_count];
    for (size_t i = 0; i < _node_count; ++i) {
      _top[i] = tree1._top[i];
    }
  }

  ~kd_tree() { delete[] _top; }

  /** Create a topolopy given a set of sinks */
  kt_node<recti::point> *create(std::vector<recti::point> sinks,
                                Direction dir = VERTICAL);
  /** Create a topolopy given the root of a routing-tree. */
  kt_node<recti::point> *create(dsl::bt_node<recti::point> *routing_root,
                                Direction dir = VERTICAL);

  /** Select points given a point(/rectangle range /two points), searching from
   * the root */
  void select_point(recti::point &s);
  void select_rect(kt_rectangle &rect);
  void select_rect(recti::point top_left, recti::point bottom_right);

  // searching from the given node of kd-tree(maybe the root of a subtree).
  void select_point(kt_node<recti::point> *top, recti::point &s);
  void select_rect(kt_node<recti::point> *top, kt_rectangle &rect);
  void select_rect(kt_node<recti::point> *top, recti::point top_left,
                   recti::point bottom_right);

  /** Report subtree */
  void report_subtree(kt_node<recti::point> *v);

protected:
  /** If sinks not given, first convert routing tree to simply sinks. */
  std::vector<rt_point> convert(dsl::bt_node<recti::point> *root);
  /** Convert routing tree to certain sinks. */
  void convert(dsl::bt_node<recti::point> *root, std::vector<rt_point> &sinks);

  /** Partition.
      @return the median position of the partition. */
  sink_it partition(const sink_it &pos1, const sink_it &pos2, Direction &dir);

  /** Search the points stored in the range R */
  void search_kd_tree(kt_node<recti::point> *v, kt_rectangle &range);
  void search_kd_tree(kt_node<recti::point> *v, kt_rectangle &region,
                      kt_rectangle &range);

  /** Get the corresponding region of v. */
  kt_rectangle get_region(kt_node<recti::point> *v);
  /** Get the corresponding region of v given the parent-region. */
  kt_rectangle get_region(kt_node<recti::point> *v, kt_rectangle parent_region);

private:
  /** Recursive call to build the tree of topology. @return the root
      of tree of current partition. */
  kt_node<recti::point> &
  build_kd_tree(const sink_it &pos1, const sink_it &pos2,
                Direction dir); // num to determine the splitting line

  //  /** Print kd-tree recursively */
  //  void print_subtree(svgstream& os, const kt_node<point>* v,
  //                     Direction dir, recti::Rectangle<int> bound) const;

public:
  // For testing:
  /** Report kd_tree.*/
  void report_subtree1(kt_node<recti::point> *v);
  kt_node<recti::point> *kd_create(kt_node<recti::point> *kdtree_root,
                                   Direction dir = VERTICAL);
  // for testing,convert from a kdtree.
  std::vector<rt_point> kd_convert(kt_node<recti::point> *root);
  void kd_convert(kt_node<recti::point> *root, std::vector<rt_point> &sinks);
  void print_selection(std::vector<kt_node<recti::point> *> points);
  size_t _rn; // number of routing-tree nodes

  void print_subtree(svgstream &os, const kt_node<recti::point> *v,
                     Direction dir, rectangle R) const {
    if (v->is_leaf()) {
      os << v->_data << "\n";
      return;
    }

    recti::rectangle R1;
    recti::rectangle R2;
    const numeric::interval<int> xb = R.x_bound();
    const numeric::interval<int> yb = R.y_bound();

    if (dir == VERTICAL) {
      // Draw vertical line
      const int x = v->_data.x();
      os << "<line"
         << " x1=\"" << x << "\""
         << " y1=\"" << yb.lower() << "\""
         << " x2=\"" << x << "\""
         << " y2=\"" << yb.upper() << "\""
         << " style=\"fill:green;stroke:green\""
         << "/>\n";
      R1 = recti::rectangle(numeric::interval<int>(xb.lower(), x), yb);
      R2 = recti::rectangle(numeric::interval<int>(x, xb.upper()), yb);
      dir = HORIZONTAL;
    } else {
      // Draw horizontal line
      const int y = v->_data.y();
      os << "<line"
         << " x1=\"" << xb.lower() << "\""
         << " y1=\"" << y << "\""
         << " x2=\"" << xb.upper() << "\""
         << " y2=\"" << y << "\""
         << " style=\"fill:blue;stroke:blue\""
         << "/>\n";
      R1 = recti::rectangle(xb, numeric::interval<int>(yb.lower(), y));
      R2 = recti::rectangle(xb, numeric::interval<int>(y, yb.upper()));
      dir = VERTICAL;
    }

    print_subtree(os, v->_left, dir, R1);
    print_subtree(os, v->_right, dir, R2);
  }
};

//####}//namespace

#endif
