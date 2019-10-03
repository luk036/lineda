#include <iostream>
#include <map>
#include <slicing_flrpln.hpp>
#include <utility.hpp>

using namespace dsl;
using namespace std;

#if defined(__GNUC__) && (__GNUC__ >= 4)
// Use TR1's unordered map for g++ 4.0 or later
#include <tr1/unordered_map>
typedef std::tr1::unordered_map<string, int> Mt;
#else
// Use old hash table for old g++ versions
#include <ext/hash_map>
// #include <ext/stl_hash_fun.h>
namespace __gnu_cxx {
/** Hash function for std::string */
template <> struct hash<string> {
  size_t operator()(const string &__s) const {
    return __stl_hash_string(__s.c_str());
  }
};
}
typedef __gnu_cxx::hash_map<string, int, __gnu_cxx::hash<string>> Mt;
#endif

/**
 * Test if the random slicing floorplan is unbiased, by using the
 * chi-square test suggested by reference~\cite{a-note-on-remy}.
 * We generate a large number of random slicing floorplan and count
 * the frequencies of each occurrence. First, we convert each
 * floorplan into its "Single Sequence" (SS) representation. We assume
 * that the number of "rooms" is less than 200 so that an SS is stored
 * in a string. Each string is further hashed into a hash table for
 * fast searching.
 * The method generates more uniform structures than the naive method.
 * However, it is still not truly uniform.
 */
int main(int argc, const char *argv[]) {
  const size_t N = (argc < 2) ? 4 : atoi(argv[1]);

  // Total number of slicing floorplans with N room is given by the
  // Shr{\"o}der number.
  const size_t numS = lineda::schroder<size_t>(N);
  std::cout << "// Sn = " << numS << std::endl;

  slicing_flrpln T(N);
  T.randomize();
  std::cout << T;

  // xxx const size_t numR = 1000;
  // xxx
  // xxx typedef Mt::iterator MtIter;
  // xxx srand((unsigned)time(NULL));  // randomize each time
  // xxx
  // xxx Mt M;  // hash table for counting
  // xxx
  // xxx // Count the frequencies of occurrence of each floorplan
  // xxx for (size_t i=0; i<numR; ++i) {
  // xxx   for (size_t j=0; j<numS; ++j) {
  // xxx     slicing_flrpln T(N);
  // xxx     T.randomize();
  // xxx     string key = T.SS().c_str();   // convert floorplan into SS
  // xxx     if (M.find(key) != M.end())    // already in hash table?
  // xxx       ++M[key];                    // increase the count
  // xxx     else
  // xxx       M[key] = 1;                  // put into hash table
  // xxx   }
  // xxx }

  // xxx // Chi-square testing
  // xxx MtIter it = M.begin();
  // xxx double V = 0.;
  // xxx for (; it!=M.end(); ++it) {
  // xxx   double diff = it->second - (int) numR;
  // xxx   V += diff*diff/numR;
  // xxx   cout << "-" << it->second;
  // xxx }
  // xxx std::cout << '\n';
  // xxx
  // xxx std::cout << "V = " << V;
  std::cout << "\n";
}
