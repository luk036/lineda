#include <iostream>

int main() {
  int *a = new int[0];
  std::cout << a << std::endl;
  std::cout << a[9332324] << std::endl;
  delete[] a;
}
