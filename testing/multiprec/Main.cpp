#include <gmpxx.h> // for mpz_class
#include <iostream>
#include <utility.hpp>

/**
 * Check if generic algorithms work with multi-precision package.
 */

using namespace std;
using namespace lineda;

int main() {
  cout << catalan_number<double>(40) << endl;
  cout << catalan_number<mpz_class>(40) << endl;
  cout << schroder<double>(40) << endl;
  cout << schroder<mpz_class>(40) << endl;
  cout << baxter<double>(40) << endl;
  cout << baxter<mpz_class>(40) << endl;
}
