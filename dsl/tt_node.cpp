#include "tt_node.hpp"
#include <boost/static_assert.hpp>

namespace dsl {

// Don't increase the size of snode_base as it is used
// as a base in many classes.
BOOST_STATIC_ASSERT(sizeof(tt_node_base) <= 4 * sizeof(void *));

// Explicit instantiations
template class tt_node<int>;
}
