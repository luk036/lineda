#include "orient.hpp"
#include <boost/static_assert.hpp>

namespace recti {

// Don't increase the size.
BOOST_STATIC_ASSERT(sizeof(orient) <= 4);

// Look up tables for rotations and flippings
const orient::Orient orient::_lut_rot090[8] = {ROT090, ROT180, ROT270, ROT000,
                                               FLI270, FLI000, FLI090, FLI180};

const orient::Orient orient::_lut_rot180[8] = {ROT180, ROT270, ROT000, ROT090,
                                               FLI180, FLI270, FLI000, FLI090};

const orient::Orient orient::_lut_rot270[8] = {ROT270, ROT000, ROT090, ROT180,
                                               FLI090, FLI180, FLI270, FLI000};

const orient::Orient orient::_lut_fli000[8] = {FLI000, FLI090, FLI180, FLI270,
                                               ROT000, ROT090, ROT180, ROT270};

const orient::Orient orient::_lut_fli090[8] = {FLI090, FLI180, FLI270, FLI000,
                                               ROT270, ROT000, ROT090, ROT180};

const orient::Orient orient::_lut_fli180[8] = {FLI180, FLI270, FLI000, FLI090,
                                               ROT180, ROT270, ROT000, ROT090};

const orient::Orient orient::_lut_fli270[8] = {FLI270, FLI000, FLI090, FLI180,
                                               ROT090, ROT180, ROT270, ROT000};
}
