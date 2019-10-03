#include "setparthlpr.hpp"
#include <iostream>

static size_t RG[512]; /**< pointer to Restricted Growth array */

/** Constructor */
setPartHlpr::setPartHlpr(size_t n, size_t k) : _n(n), _k(k), _cnt(0) {
  assert(k >= 2);
  assert(n >= k);
}

/** Destructor */
setPartHlpr::~setPartHlpr() {}

/** Run the partition */
void setPartHlpr::run() {
  _cnt = 0;
  ++_cnt;
  process_init();
  if (_k & 1)
    GEN0_odd(_n, _k);
  else
    GEN0_even(_n, _k);
  assert(stirling2nd(_n, _k) == _cnt);
}

/** Default processing function */
void setPartHlpr::process_init() {
  size_t i, j;

  for (i = 0; i < _n - _k; ++i)
    RG[i] = 0;
  for (j = 0; i < _n; ++i, ++j)
    RG[i] = j;
  std::cout << _cnt << ": ";
  for (i = 0; i < _n; ++i)
    std::cout << (size_t)RG[i];
  std::cout << std::endl;
}

/** Default processing function */
void setPartHlpr::process(size_t x, size_t y) {
  size_t i;

  std::cout << _cnt << ": ";
  size_t old = RG[x];
  assert(old != y); // check if there is any implementation error

  RG[x] = y;
  for (i = 0; i < _n; ++i)
    std::cout << (size_t)RG[i];
  std::cout << "   moved element " << x << " from block " << (size_t)old
            << " to " << (size_t)y;
  std::cout << std::endl;
}

// Stirling number of second kind. Note that this function is for
// testing purpose only and is slow because of reducdant calculation.
size_t setPartHlpr::stirling2nd(size_t n, size_t k) {
  if (k >= n || k <= 1)
    return 1;
  return stirling2nd(n - 1, k - 1) + k * stirling2nd(n - 1, k);
}

// The lists S(n,k,0) and S(n,k,1) satisfy the following properties.
// 1. Successive RG sequences differ in exactly one position.
// 2. first(S(n,k,0)) = first(S(n,k,1)) = 0^{n-k}0123...(k-1)
// 3. last(S(n,k,0)) = 0^{n-k}12...(k-1)0
// 4. last(S(n,k,1)) = 012...(k-1)0^{n-k}
// Note that first(S'(n,k,p)) = last(S(n,k,p))

// S(n,k,0) even k
void setPartHlpr::GEN0_even(size_t n, size_t k) {
  if (!(k > 1 && k < n))
    return;

  GEN0_odd(n - 1, k - 1); // S(n-1, k-1, 0).(k-1)
  move(n - 1, k - 1);
  GEN1_even(n - 1, k); // S(n-1, k, 1).(k-1)
  move(n, k - 2);
  NEG1_even(n - 1, k); // S'(n-1, k, 1).(k-2)

  for (int i = k - 3; i >= 1; i -= 2) {
    move(n, i);
    GEN1_even(n - 1, k); // S(n-1, k, 1).i
    move(n, i - 1);
    NEG1_even(n - 1, k); // S'(n-1, k, 1).(i-1)
  }
}

// S'(n, k, 0) even k
void setPartHlpr::NEG0_even(size_t n, size_t k) {
  if (!(k > 1 && k < n))
    return;

  for (int i = 1; i <= (int)k - 3; i += 2) {
    GEN1_even(n - 1, k); // S(n-1, k, 1).(i-1)
    move(n, i);
    NEG1_even(n - 1, k); // S'(n-1, k, 1).i
    move(n, i + 1);
  }

  GEN1_even(n - 1, k); // S(n-1, k, 1).(k-2)
  move(n, k - 1);
  NEG1_even(n - 1, k); // S(n-1, k, 1).(k-1)
  move(n - 1, 0);
  NEG0_odd(n - 1, k - 1); // S(n-1, k-1, 1).(k-1)
}

// S(n, k, 1) even k
void setPartHlpr::GEN1_even(size_t n, size_t k) {
  if (!(k > 1 && k < n))
    return;

  GEN1_odd(n - 1, k - 1);
  move(k, k - 1);
  NEG1_even(n - 1, k);
  move(n, k - 2);
  GEN1_even(n - 1, k);

  for (int i = k - 3; i > 0; i -= 2) {
    move(n, i);
    NEG1_even(n - 1, k);
    move(n, i - 1);
    GEN1_even(n - 1, k);
  }
}

// S'(n, k, 1) even k
void setPartHlpr::NEG1_even(size_t n, size_t k) {
  if (!(k > 1 && k < n))
    return;

  for (int i = 1; i <= (int)k - 3; i += 2) {
    NEG1_even(n - 1, k);
    move(n, i);
    GEN1_even(n - 1, k);
    move(n, i + 1);
  }

  NEG1_even(n - 1, k);
  move(n, k - 1);
  GEN1_even(n - 1, k);
  move(k, 0);
  NEG1_odd(n - 1, k - 1);
}

// S(n, k, 0) odd k
void setPartHlpr::GEN0_odd(size_t n, size_t k) {
  if (!(k > 1 && k < n))
    return;

  GEN1_even(n - 1, k - 1);
  move(k, k - 1);
  NEG1_odd(n - 1, k);

  for (int i = k - 2; i > 0; i -= 2) {
    move(n, i);
    GEN1_odd(n - 1, k);
    move(n, i - 1);
    NEG1_odd(n - 1, k);
  }
}

// S'(n, k, 0) odd k
void setPartHlpr::NEG0_odd(size_t n, size_t k) {
  if (!(k > 1 && k < n))
    return;

  for (int i = 1; i <= (int)k - 2; i += 2) {
    GEN1_odd(n - 1, k);
    move(n, i);
    NEG1_odd(n - 1, k);
    move(n, i + 1);
  }

  GEN1_odd(n - 1, k);
  move(k, 0); // ???
  NEG1_even(n - 1, k - 1);
}

// S(n, k, 1) odd k
void setPartHlpr::GEN1_odd(size_t n, size_t k) {
  if (!(k > 1 && k < n))
    return;

  GEN0_even(n - 1, k - 1);
  move(n - 1, k - 1);
  GEN1_odd(n - 1, k);

  for (int i = k - 2; i > 0; i -= 2) {
    move(n, i);
    NEG1_odd(n - 1, k);
    move(n, i - 1);
    GEN1_odd(n - 1, k);
  }
}

// S'(n, k, 1) odd k
void setPartHlpr::NEG1_odd(size_t n, size_t k) {
  if (!(k > 1 && k < n))
    return;

  for (int i = 1; i <= (int)k - 2; i += 2) {
    NEG1_odd(n - 1, k);
    move(n, i);
    GEN1_odd(n - 1, k);
    move(n, i + 1);
  }

  NEG1_odd(n - 1, k);
  move(n - 1, 0);
  NEG0_even(n - 1, k - 1);
}
