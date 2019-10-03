/**
 * @file functions.hpp
 * @author why(Hanyi Wei)
 * @date Sep.14, 2009
 *
 * @brief used to do some pre-dealing to each rpolygon
 * (a polygon whose angles are all rectangles, in lineda)
 * in the input list<rpolygon>
 * cut each rpolygon into rectangles
 * results are put into vector<rectangle>
 */
#ifndef CUTTING_FUNCTIONS_HPP
#define CUTTING_FUNCTIONS_HPP

#include "rectangle.hpp"
#include "rpolygon.hpp"
#include <list>
#include <vector>

using namespace recti;
using namespace std;

/** @brief used to do some pre-dealing to each rpolygon
 *  in the input list<rpolygon>
 *  build Pn[] and corredponding Polygonc object to cut
 *  each rpolygon into rectangles
 *  results are put into vector<rectangle>
 *  used in main()
 */
void polygon_cutting(list<rpolygon> &rp, vector<rectangle> &R);

#endif
