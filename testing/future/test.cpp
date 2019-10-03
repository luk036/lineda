#include <future>
#include <iostream>

int a, b, c;
int calculateA() { return a + a * b; }
int calculateB() { return a * (a + a * (a + 1)); }
int calculateC() { return b * (b + 1) - b; }
int main() {
  using namespace std;
  future<int> f1 = async(calculateB), f2 = async(calculateC);
  c = (calculateA() + f1.get()) * f2.get();
  std::cout << c << std::endl;
}
