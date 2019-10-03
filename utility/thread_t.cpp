#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

boost::mutex io_mutex;

void count(int id, int &id2) {
  for (int i = 0; i < 10; ++i) {
    boost::mutex::scoped_lock lock(io_mutex);
    std::cout << id << ": " << id2 << " " << i << std::endl;
  }
}

int main(int argc, char *argv[]) {
  int b = 3;

  boost::thread thrd1(boost::bind(&count, 1, boost::ref(b)));
  boost::thread thrd2(boost::bind(&count, 2, boost::ref(b)));
  thrd1.join();
  thrd2.join();
  return 0;
}
