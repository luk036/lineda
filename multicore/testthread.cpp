#include <iostream>
#include <thread>
#include <algorithm>
#include <cassert>

void thFun(int& i)
{
  std::cout << "Hi from thread " << i << "!\n";
}
	
int main()
{
  std::vector<std::thread> workers;
  for (int i=0; i<10; ++i)
  {
    auto th = std::thread(&thFun, std::ref(i));
    workers.push_back(std::move(th));
	assert(!th.joinable());
  }
  std::cout << "Hi from main!\n";
  std::for_each(workers.begin(), workers.end(), [](std::thread & th)
  {
    assert(th.joinable());
    th.join();
  });
  return 0;
}