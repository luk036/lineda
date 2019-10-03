#ifndef DSL_SLICING_FLRPLN_HPP
#define DSL_SLICING_FLRPLN_HPP

#include "bin_tree.hpp"
#include "bt_node.hpp"
#include <cassert>
#include <iosfwd>
#include <stdexcept> // for std::domain_error
#include <string>

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Class that is thrown when non-canonical floorplan is found */
class non_canonical_flrpln : public std::domain_error {
public:
  explicit non_canonical_flrpln()
      : std::domain_error("non-canonical floorplan") {}
};

/**
 * A slicing floorplan (implemented by slicing tree). The algorithm
 * will enumerates all the slicing floorplans by minimum local
 * changes each time.
 *   Reference :
 *     M. Sarrafzadeh and C. K. Wong. An Introduction to VLSI
 *   Physical Design, McGraw Hill, 1996, p.59-60.
 */
class slicing_flrpln : public bin_tree_base {
  typedef slicing_flrpln _Self;
  typedef bin_tree_base _Base;
  typedef bgt_node _Node;
  typedef _Base::_Node _Node_Base;

  friend std::ostream &operator<<(std::ostream &os, const _Self &flrpln);

public:
  /** Create a slicing floorplan with n rooms. Precondtion: n > 0. */
  slicing_flrpln(size_t n);

  /** Destructor */
  ~slicing_flrpln() override;

  // xxx /** Create the next floorplan structure.  */
  // xxx inline void create_next_flrpln() { create_next_tree(); }

  /** Check whether the subtree pointed at t is in the canonical
      form. An exception "non_canonical_flrpln" will be thrown if it
      is not. A slicing floorplan may have different slicing tree
      representations. The problem arises when there are more than
      one slice that can cut the floorplan. The problem can be
      easily solved by giving priority to the slices: from
      right-(bottom-) most slice is chosen. This property means that
      the orientation of a parent node must be differet from that of
      its right child. In corresponding Polish expression of this
      slicing tree, no two consecutive operators are identical. This
      is called normalized Polish expression. There is a one-to-one
      correspondence between a slicing floorplan and its normalized
      Polish expression. */
  void check_canonical(const bgt_node *t) const;

  /** @return if it is in canonical form */
  bool is_canonical() const;

  /** Generate random slicing floorplan in canonical form. */
  void randomize();

  /** Print out the floorplan structure in .dot format to os */
  void print(std::ostream &os, size_t &c) const;

  /** @return Single Sequence representation in std::string */
  std::string SS() const;

private:
  // Degnerate copy constructor and assignment operator
  slicing_flrpln(const _Self &) = delete;
  _Self &operator=(const _Self &) = delete;

  void rand_dir(bgt_node *t);

  /** Print out the partial floorplan structure in .dot format to os
      (recursively function) */
  void print_it(std::ostream &os, const bgt_node *t, size_t &d,
                bool direct) const;

  void SS_recur1(const bgt_node *t, char &c) const;

  void SS_recur(std::string &ss, const bgt_node *t) const;

  // xxx /** Print out the floorplan in .dot format if it is in canonical
  // xxx 	form then flip the orientation bit according to the BRGC order.
  // */
  // xxx void flip(std::ostream& os, size_t n, size_t& c) const;
  // xxx
  // xxx /**
  // xxx  * Print out all the canonical floorplans in .dot format (given
  // xxx  * one particular slicing tree structure) according to the
  // xxx  * Binary Reflected Gray Code order (recursive function). The
  // xxx  * transition sequence of the BRGC is the sequence of positions
  // xxx  * of the bits that change. For n = 3 it is
  // xxx  * 1,2,1,3,1,2,1. Inductively, the sequence is defined as T(n+1)
  // xxx  * = T(n),n+1,T(n), with T(1) = 1. It is this sequence that is
  // xxx  * output when "transposition or bit change" output is selected.
  // xxx  * @see http://www.theory.csc.uvic.ca/~cos/inf/comb/SubsetInfo.html
  // xxx  */
  // xxx void BRGC(std::ostream& os, size_t n, size_t& c) const;

  _Node *const _bgt_a; /**< array of pointer of nodes */
};

/**@} */
}

#endif
