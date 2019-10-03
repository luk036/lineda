#include <iostream>
#include <string>
#include <thread>
#include <future>


std::string fun()
{
    std::string str("Hello from future!"); 
	throw(std::exception("Exception from task!");
	return str;
}

int main()
{
  auto ftr = std::async(&fun);
  std::cout << "Hello from main!\n";
  try
  {
    std::string str = ftr.get();
	std::cout << str << std::endl;
  }
  catch(std::exception& e)
  {
  }