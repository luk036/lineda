/*
 * aa_aaf.h -- Affine Arithmetic class
 * Copyright (C) 2003 EPFL (Ecole Polytechnique Federale de Lausanne)
 * Copyright (c) 2004 LIRIS (University Claude Bernard Lyon 1)
 * Copyright (c) 2005 Nathan Hurst
 *
 * This file is part of libaffa.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libaffa; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef AA_AAF_H
#define AA_AAF_H

#include "aa_interval.h"
#include <iostream>
#include <cmath>
#include <map>

typedef enum AAF_TYPE{
    AAF_TYPE_EMPTY=0,
    AAF_TYPE_AFFINE=1,
    AAF_TYPE_INFINITE=2,
    AAF_TYPE_NAN=4
} AAF_TYPE;

// Affine Arithmetic Form

namespace recti {
template <typename T>
class Zonotope;
}

class AAF
{
    friend class recti::Zonotope<double>;

private:
    AAF_TYPE special{AAF_TYPE_AFFINE}; // infinite, nan
    static unsigned last;  // highest noise symbol in use

    double cvalue;       // central value vo
    unsigned length{0};         // lenght of indexes

    // At creation we don't store null coefficients

    double * coefficients{nullptr}; // values of noise sym
    unsigned * indexes{nullptr};   // indexes of noise sym

public:

    AAF(AAF_TYPE t);
    AAF(double v0 = 0);
    AAF(double v0, const double * t1, const unsigned * t2, unsigned T);
    AAF(const AAF & P);
    
 // affine constructor
    AAF(const AAF & P, double alpha, double dzeta, double delta, AAF_TYPE type);
    AAF(interval iv);
    ~AAF();

    AAF & operator =(const AAF & P);
    AAF operator +(const AAF & P) const;
    AAF operator -(const AAF & P) const;
    AAF operator *(const AAF & P) const;
    AAF operator /(const AAF & P) const;

    bool operator ==(const AAF & P) const;
    bool operator !=(const AAF & P) const;
    //xxx bool operator <(const AAF & P) const; 
    //xxx bool operator >(const AAF & P) const; 
    //xxx bool operator <=(const AAF & P) const; 
    //xxx bool operator >=(const AAF & P) const; 
    AAF & operator +=(const double cst);
    AAF & operator -=(const double cst);
    AAF & operator *=(const double cst);
    AAF & operator /=(const double cst);
    AAF & operator +=(const AAF & P);
    AAF & operator -=(const AAF & P);
    AAF & operator *=(const AAF & P);
    AAF & operator /=(const AAF & P);//added on march 13 2006 by J.Fang

    friend AAF abs(const AAF & P);
    friend AAF sqrt(const AAF & P);
    friend AAF inv(const AAF & P);
    friend AAF exp(const AAF & P);
    friend AAF log(const AAF & P);
    friend AAF sin(const AAF & P);
    friend AAF pow(const AAF & P, double exp);
    friend AAF half_plane(const AAF & P);
    
    AAF operator -() const;
    AAF operator *(double) const;

    // Added by luk
    inline AAF operator /(double cst) const { return operator*(1./cst); }
 
    friend std::ostream & operator <<(std::ostream & s, const AAF &P);

    void aafprint() const;
    static void set_default(const unsigned val=0);
    static unsigned inclast();
    unsigned get_length() const;
    double get_center() const;
    interval convert() const;
    double rad() const;
    double get_coeff(unsigned i) const {
        if(i >= length)
            return 0;
        return coefficients[i];
    }
    unsigned get_index(unsigned i) const {
        if(i >= length)
            return 0;
        return indexes[i];
    }
    bool is_indeterminate() const;
    bool is_infinite() const;
    bool is_nan() const;
    bool straddles_zero() const;
    bool strictly_neg() const;
    AAF_TYPE get_special() const {
        return special;
    }

    //xxx // Added by luk
    //xxx // Obtain the maximum possible value, record the polarity in P
    //xxx double max(AAF& P) const;
    //xxx 
    //xxx // Evaluate a possible value of this AAF, using the polarity P
    //xxx double eval(const AAF& P) const;
    //xxx 
    //xxx // Added by luk
    //xxx // Obtain the maximum possible value, record the polarity in P
    //xxx double maximum(std::map<unsigned,int>& S) const;
    //xxx 
    //xxx // Evaluate a possible value of this AAF, using the polarity P
    //xxx double evaluate(const std::map<unsigned,int>& S) const;

};

AAF operator *(const double, const AAF);
AAF operator +(const double, const AAF);
AAF operator -(const double, const AAF);

// Added by luk:
inline AAF operator /(const double cst, const AAF a) 
{ return AAF(cst) / a; }

/** Define a < b is true if there exists element a(i) < b(j) */
namespace aaf_possible_compare
{
  bool operator<(const AAF& lhs, double rhs);
  bool operator>(const AAF& lhs, double rhs);
//xxx bool operator<=(const AAF& lhs, double rhs);
//xxx bool operator>=(const AAF& lhs, double rhs);
  bool operator<(const AAF& lhs, const AAF& rhs);
  bool operator>(const AAF& lhs, const AAF& rhs);
  bool operator<=(const AAF& lhs, const AAF& rhs);
  bool operator>=(const AAF& lhs, const AAF& rhs);
}

/** Define a < b is true if for all elements, a(i) < b(j) */
namespace aaf_absolute_compare
{
  bool operator<(const AAF& lhs, double rhs);
  bool operator>(const AAF& lhs, double rhs);
//xxx bool operator<=(const AAF& lhs, double rhs);
//xxx bool operator>=(const AAF& lhs, double rhs);
  bool operator<(const AAF& lhs, const AAF& rhs);
  bool operator>(const AAF& lhs, const AAF& rhs);
  bool operator<=(const AAF& lhs, const AAF& rhs);
  bool operator>=(const AAF& lhs, const AAF& rhs);
}

AAF abs(const AAF & P);
AAF sqr(const AAF & P);
AAF exp(const AAF & P);
AAF log(const AAF & P);
AAF pow(const AAF & P, int exp);
AAF pow(const AAF & P, double exp);
AAF cos(const AAF & P);
AAF tan(const AAF & P);
AAF cotan(const AAF);

AAF cosh(const AAF & P);
AAF sinh(const AAF & P);
AAF tanh(const AAF & P);
AAF acosh(const AAF & P);
AAF asinh(const AAF & P);
AAF atanh(const AAF & P);

AAF half_plane(const AAF & P);

// AAF inline functions

// Create a constant AAF of v0

inline AAF:: AAF(double v0):
    cvalue(v0) 
{
}

inline AAF:: AAF(AAF_TYPE t):
    special(t),cvalue(0), length(0),
    coefficients(nullptr), indexes(nullptr)
{
}


// Says highest symbol in use is val

inline void AAF:: set_default(const unsigned val) {
    last=val;
}


// Increment the highest symbol in use
// i.e. create a new noise symbol

inline unsigned AAF:: inclast() {
    return ++last;
}


// Get the length of an AAF
// i.e the number of non-null noise symbols

inline unsigned AAF::get_length() const {
    return length;
}

inline bool AAF::is_indeterminate() const {
    if(special & AAF_TYPE_INFINITE) return true;
    if(special & AAF_TYPE_NAN) return true;
    return rad() == INFINITY;
}

inline bool AAF::is_infinite() const {
    if(special & AAF_TYPE_INFINITE) return true;
    return rad() == INFINITY;
}

inline bool AAF::is_nan() const {
    if(special & AAF_TYPE_NAN) return true;
    return false;
}


inline bool AAF::straddles_zero() const {
    return convert().straddles_zero();
}


inline bool AAF::strictly_neg() const {
    return convert().right() < 0;
}


#endif  // AA_AAF_H
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/


// vim: filetype=c++:expandtab:shiftwidth=4:tabstop=8:softtabstop=4 :
