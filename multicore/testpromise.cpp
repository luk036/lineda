#include <iostream>
#include <string>
#include <thread>
#include <future>

void thFun(std::promise<string>&& prms);
{
  try
  {
    std::string str("Hello from future!"); 
	throw(std::exception("Exception from future!"));
    prms.set_value(str);
  }
  catch(...)
  {
     prms.set_exception(std::current_exception());
  }
}

int main()
{
  std::promise<std::string> prms;
  std::future<std::string> ftr = prms.get_future();
  std::thread th(&thFun, std::move(prms));
  std::cout << "Hello from main!\n");
  try
  {
    std::string str = ftr.get();
	std::cout << str << std::endl;
  }
  catch ()
  {
    Exception from future

 