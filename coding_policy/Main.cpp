#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

// MDR: Only file that contains "main()" can redefine the
// global "new" and "delete" operators.
int main() {
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
  runner.run("", false); // Run all tests and wait
  return 0;
}
