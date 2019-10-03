#ifndef SVG_STREAM_HPP
#define SVG_STREAM_HPP

#include <fstream>
#include <iostream>

/**
 * @addtogroup utility
 * @{
 */

/** This class is for writing out SVG files. */
class svgstream : public std::ofstream {
public:
  /** Constructor */
  svgstream(const char *filename, int width = 2000, int height = 2000)
      : std::ofstream(filename) {
    std::ofstream &os = *this;
    os << "<?xml version=\"1.0\" standalone=\"no\"?>\n"
          "<!DOCTYPE svg PUBLIC \"-//W3C/DTD SVG 1.1//EN\"\n"
          "  \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
          "<svg width=\"500px\" height=\"500px\" viewBox=\"0 0 "
       << width << " " << height
       << "\"\n"
          "  xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
          "  <desc>Generate from recti package</desc>\n"
          "  <g transform=\"translate(1000,1000)\">\n";
  }

  /** Destructor */
  ~svgstream() override {
    std::ofstream &os = *this;
    os << "  </g>\n</svg>";
  }
};

/** @} */

#endif
