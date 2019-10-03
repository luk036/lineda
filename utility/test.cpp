#include <iostream>
#include <vector>

using namespace std;

int main() {
  int N = 50;
  int M = 60;

  vector<vector<int> > A(N, vector<int>(M));

  A[1][2] = 3;

  std::cout << A[1][2] << ' ';
  std::cout << std::endl;
}
