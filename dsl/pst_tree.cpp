#include "pst_tree.hpp"

namespace std {
int __black_count(_Pst_tree_node_base *__node, _Pst_tree_node_base *__root) {
  if (__node == nullptr)
    return 0;
  int __sum = 0;
  do {
    if (__node->_M_color == _Pst_M_black)
      ++__sum;
    if (__node == __root)
      break;
    __node = __node->_M_parent;
  } while (true);
  return __sum;
}

void _Pst_tree_rotate_left(_Pst_tree_node_base *__x,
                           _Pst_tree_node_base *&__root) {
  _Pst_tree_node_base *__y = __x->_M_right;
  __x->_M_right = __y->_M_left;
  assert(__y->_M_left != 0);
  __y->_M_left->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_left)
    __x->_M_parent->_M_left = __y;
  else
    __x->_M_parent->_M_right = __y;
  __y->_M_left = __x;
  __x->_M_parent = __y;
}

/* Same as _Pst_tree_rotate_left with "right" and "left" exchange */
void _Pst_tree_rotate_right(_Pst_tree_node_base *__x,
                            _Pst_tree_node_base *&__root) {
  _Pst_tree_node_base *__y = __x->_M_left;
  __x->_M_left = __y->_M_right;
  assert(__y->_M_right != 0);
  __y->_M_right->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_right)
    __x->_M_parent->_M_right = __y;
  else
    __x->_M_parent->_M_left = __y;
  __y->_M_right = __x;
  __x->_M_parent = __y;
}

// xxx   void
// xxx   _Pst_tree_rebalance(_Pst_tree_node_base* __x, _Pst_tree_node_base*&
// __root)
// xxx   {
// xxx     __x->_M_color = _Pst_M_red;
// xxx     while (__x != __root
// xxx 	   && __x->_M_parent->_M_color == _Pst_M_red)
// xxx       {
// xxx 	if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left)
// xxx 	  {
// xxx 	    _Pst_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
// xxx 	    if (__y && __y->_M_color == _Pst_M_red)
// xxx 	      {
// xxx 		__x->_M_parent->_M_color = _Pst_M_black;
// xxx 		__y->_M_color = _Pst_M_black;
// xxx 		__x->_M_parent->_M_parent->_M_color = _Pst_M_red;
// xxx 		__x = __x->_M_parent->_M_parent;
// xxx 	      }
// xxx 	    else
// xxx 	      {
// xxx 		if (__x == __x->_M_parent->_M_right)
// xxx 		  {
// xxx 		    __x = __x->_M_parent;
// xxx 		    _Pst_tree_rotate_left(__x, __root);
// xxx 		  }
// xxx 		__x->_M_parent->_M_color = _Pst_M_black;
// xxx 		__x->_M_parent->_M_parent->_M_color = _Pst_M_red;
// xxx 		_Pst_tree_rotate_right(__x->_M_parent->_M_parent, __root);
// xxx 	      }
// xxx 	  }
// xxx 	else /* same as "if" clause with "left" and "right" exchange */
// xxx 	  {
// xxx 	    _Pst_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
// xxx 	    if (__y && __y->_M_color == _Pst_M_red)
// xxx 	      {
// xxx 		__x->_M_parent->_M_color = _Pst_M_black;
// xxx 		__y->_M_color = _Pst_M_black;
// xxx 		__x->_M_parent->_M_parent->_M_color = _Pst_M_red;
// xxx 		__x = __x->_M_parent->_M_parent;
// xxx 	      }
// xxx 	    else
// xxx 	      {
// xxx 		if (__x == __x->_M_parent->_M_left)
// xxx 		  {
// xxx 		    __x = __x->_M_parent;
// xxx 		    _Pst_tree_rotate_right(__x, __root);
// xxx 		  }
// xxx 		__x->_M_parent->_M_color = _Pst_M_black;
// xxx 		__x->_M_parent->_M_parent->_M_color = _Pst_M_red;
// xxx 		_Pst_tree_rotate_left(__x->_M_parent->_M_parent, __root);
// xxx 	      }
// xxx 	  }
// xxx       }
// xxx     __root->_M_color = _Pst_M_black;
// xxx   }
// xxx
// xxx   _Pst_tree_node_base*
// xxx   _Pst_tree_rebalance_for_erase(_Pst_tree_node_base* __z,
// xxx 			       _Pst_tree_node_base*& __root,
// xxx 			       _Pst_tree_node_base*& __leftmost,
// xxx 			       _Pst_tree_node_base*& __rightmost)
// xxx   {
// xxx     assert(__z->_M_left==0 || __z->_M_right==0 || !"only delete leave
// nodes");
// xxx
// xxx     _Pst_tree_node_base* __y = __z;
// xxx     _Pst_tree_node_base* __x = 0;
// xxx     _Pst_tree_node_base* __x_parent = 0;
// xxx
// xxx     if (__y->_M_left == 0)     // __z has at most one non-null child. y
// == z.
// xxx       __x = __y->_M_right;     // __x might be null.
// xxx     else
// xxx       if (__y->_M_right == 0)  // __z has exactly one non-null child. y
// == z.
// xxx 	__x = __y->_M_left;    // __x is not null.
// xxx //xxx       else
// xxx //xxx 	{
// xxx //xxx 	  // __z has two non-null children.  Set __y to
// xxx //xxx 	  __y = __y->_M_right;   //   __z's successor.  __x might be
// null.
// xxx //xxx 	  while (__y->_M_left != 0)
// xxx //xxx 	    __y = __y->_M_left;
// xxx //xxx 	  __x = __y->_M_right;
// xxx //xxx 	}
// xxx //xxx     if (__y != __z)
// xxx //xxx       {
// xxx //xxx 	// relink y in place of z.  y is z's successor
// xxx //xxx 	__z->_M_left->_M_parent = __y;
// xxx //xxx 	__y->_M_left = __z->_M_left;
// xxx //xxx 	if (__y != __z->_M_right)
// xxx //xxx 	  {
// xxx //xxx 	    __x_parent = __y->_M_parent;
// xxx //xxx 	    if (__x) __x->_M_parent = __y->_M_parent;
// xxx //xxx 	    __y->_M_parent->_M_left = __x;   // __y must be a child of
// _M_left
// xxx //xxx 	    __y->_M_right = __z->_M_right;
// xxx //xxx 	    __z->_M_right->_M_parent = __y;
// xxx //xxx 	  }
// xxx //xxx 	else
// xxx //xxx 	  __x_parent = __y;
// xxx //xxx 	if (__root == __z)
// xxx //xxx 	  __root = __y;
// xxx //xxx 	else if (__z->_M_parent->_M_left == __z)
// xxx //xxx 	  __z->_M_parent->_M_left = __y;
// xxx //xxx 	else
// xxx //xxx 	  __z->_M_parent->_M_right = __y;
// xxx //xxx 	__y->_M_parent = __z->_M_parent;
// xxx //xxx 	std::swap(__y->_M_color, __z->_M_color);
// xxx //xxx 	__y = __z;
// xxx //xxx 	// __y now points to node to be actually deleted
// xxx //xxx       }
// xxx //xxx     else
// xxx //xxx       {                        // __y == __z
// xxx 	__x_parent = __y->_M_parent;
// xxx 	if (__x)
// xxx 	  __x->_M_parent = __y->_M_parent;
// xxx 	if (__root == __z)
// xxx 	  __root = __x;
// xxx 	else
// xxx 	  if (__z->_M_parent->_M_left == __z)
// xxx 	    __z->_M_parent->_M_left = __x;
// xxx 	  else
// xxx 	    __z->_M_parent->_M_right = __x;
// xxx 	if (__leftmost == __z)
// xxx 	  if (__z->_M_right == 0)        // __z->_M_left must be null also
// xxx 	    __leftmost = __z->_M_parent;
// xxx 	// makes __leftmost == _M_header if __z == __root
// xxx 	  else
// xxx 	    __leftmost = _Pst_tree_node_base::_S_minimum(__x);
// xxx 	if (__rightmost == __z)
// xxx 	  if (__z->_M_left == 0)         // __z->_M_right must be null also
// xxx 	    __rightmost = __z->_M_parent;
// xxx 	// makes __rightmost == _M_header if __z == __root
// xxx 	  else                      // __x == __z->_M_left
// xxx 	    __rightmost = _Pst_tree_node_base::_S_maximum(__x);
// xxx //xxx       }
// xxx
// xxx
// xxx     if (__y->_M_color != _Pst_M_red)
// xxx       {
// xxx         /* DELETE-FIXUP */
// xxx 	while (__x != __root && (__x == 0 || __x->_M_color == _Pst_M_black))
// xxx 	  if (__x == __x_parent->_M_left)
// xxx 	    {
// xxx 	      _Pst_tree_node_base* __w = __x_parent->_M_right;
// xxx 	      if (__w->_M_color == _Pst_M_red)
// xxx 		{
// xxx 		  __w->_M_color = _Pst_M_black;
// xxx 		  __x_parent->_M_color = _Pst_M_red;
// xxx 		  _Pst_tree_rotate_left(__x_parent, __root);
// xxx 		  __w = __x_parent->_M_right;
// xxx 		}
// xxx 	      if ((__w->_M_left == 0 ||
// xxx 		   __w->_M_left->_M_color == _Pst_M_black) &&
// xxx 		  (__w->_M_right == 0 ||
// xxx 		   __w->_M_right->_M_color == _Pst_M_black))
// xxx 		{
// xxx 		  __w->_M_color = _Pst_M_red;
// xxx 		  __x = __x_parent;
// xxx 		  __x_parent = __x_parent->_M_parent;
// xxx 		}
// xxx 	      else
// xxx 		{
// xxx 		  if (__w->_M_right == 0
// xxx 		      || __w->_M_right->_M_color == _Pst_M_black)
// xxx 		    {
// xxx 		      if (__w->_M_left) __w->_M_left->_M_color = _Pst_M_black;
// xxx 		      __w->_M_color = _Pst_M_red;
// xxx 		      _Pst_tree_rotate_right(__w, __root);
// xxx 		      __w = __x_parent->_M_right;
// xxx 		    }
// xxx 		  __w->_M_color = __x_parent->_M_color;
// xxx 		  __x_parent->_M_color = _Pst_M_black;
// xxx 		  if (__w->_M_right)
// xxx 		    __w->_M_right->_M_color = _Pst_M_black;
// xxx 		  _Pst_tree_rotate_left(__x_parent, __root);
// xxx 		  break;
// xxx 		}
// xxx 	    }
// xxx 	  else
// xxx 	    {
// xxx 	      // same as "if" clause, with "right" <-> "left".
// xxx 	      _Pst_tree_node_base* __w = __x_parent->_M_left;
// xxx 	      if (__w->_M_color == _Pst_M_red)
// xxx 		{
// xxx 		  __w->_M_color = _Pst_M_black;
// xxx 		  __x_parent->_M_color = _Pst_M_red;
// xxx 		  _Pst_tree_rotate_right(__x_parent, __root);
// xxx 		  __w = __x_parent->_M_left;
// xxx 		}
// xxx 	      if ((__w->_M_right == 0 ||
// xxx 		   __w->_M_right->_M_color == _Pst_M_black) &&
// xxx 		  (__w->_M_left == 0 ||
// xxx 		   __w->_M_left->_M_color == _Pst_M_black))
// xxx 		{
// xxx 		  __w->_M_color = _Pst_M_red;
// xxx 		  __x = __x_parent;
// xxx 		  __x_parent = __x_parent->_M_parent;
// xxx 		}
// xxx 	      else
// xxx 		{
// xxx 		  if (__w->_M_left == 0 || __w->_M_left->_M_color ==
// _Pst_M_black)
// xxx 		    {
// xxx 		      if (__w->_M_right) __w->_M_right->_M_color = _Pst_M_black;
// xxx 		      __w->_M_color = _Pst_M_red;
// xxx 		      _Pst_tree_rotate_left(__w, __root);
// xxx 		      __w = __x_parent->_M_left;
// xxx 		    }
// xxx 		  __w->_M_color = __x_parent->_M_color;
// xxx 		  __x_parent->_M_color = _Pst_M_black;
// xxx 		  if (__w->_M_left)
// xxx 		    __w->_M_left->_M_color = _Pst_M_black;
// xxx 		  _Pst_tree_rotate_right(__x_parent, __root);
// xxx 		  break;
// xxx 		}
// xxx 	    }
// xxx 	if (__x) __x->_M_color = _Pst_M_black;
// xxx       }
// xxx     return __y;
// xxx   }
// xxx
}
