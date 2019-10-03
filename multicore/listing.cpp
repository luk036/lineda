#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include "filesystem.h"

typedef std::vector<std::string> string_vector;

string_vector listDirectory(std::string && dir)
{
    string_vector listing;
	std::string dirStr("\n> ");
	dirStr += dir;
	dirStr += ":\n ";
	listing.push_back(dirStr);
	
	std::vector<std::future<string_vector>> futures;
	for (directory_iterator it(dir); it != directory_iterator(); ++it)
	{
	    if (it->is_directory())
		{
		    auto ftr = std::async(std::lauch::async, &listDirectory, it->path().file_string());
			futures.push_back(std::move(ftr));
		}
		else
		{
		    listing.push_back(it->path().leaf());
		}
	}
	std::for_each(futures.begin(), futures.end(), [&listing](std::future<string_vector> & ftr)
	{
	    string_vector lst = ftr.get();
		std::copy(lst.begin(), lst.end(), std::back_inserter(listing));
	});
	
	return listing;
}

void test1()
{
    auto startTime = std::chrono::system_clock::now();
	// ...
	auto endTime = std::chrono::system_clock::now();
	auto dur = endTime - startTime;
	auto durMs = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
	std::cout << "\ntime: " << durMs.count() <<  " milliseconds.\n";
	
	
int main()
{
    sd::string root = "c:\\projects";
	auto ftr = std::async(std::launch
}