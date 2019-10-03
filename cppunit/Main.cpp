#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cstdlib>

int main() {
  std::srand((unsigned)time(nullptr));
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
  runner.run("", false); // Run all tests and wait
  return 0;
}
