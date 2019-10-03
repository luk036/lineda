#ifndef DOT_STREAM_HPP
#define DOT_STREAM_HPP

#include <fstream>
#include <iostream>

/**
 * @addtogroup utility
 * @{
 */

/** This class is for writing out DOT files. */
class dotstream : public std::ofstream {
public:
  /** Constructor */
  dotstream(char *filename) : std::ofstream(filename) {
    std::ofstream &os = *this;
    os << "digraph \"G\""
       << " {\n";
  }

  /** Destructor */
  ~dotstream() override {
    std::ofstream &os = *this;
    os << "}\n";
  }
};

/** @} */

#endif
