#include "filesystem.h"
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include <deque>
#include <condition_variable>
const int NUM_THREADS = 10;

template <class T>
class MsgQueue
{
    std::deque<T> _queue;
	std::condition_variable _cond;
	std::mutex _mutex;
public:
    void send(T && msg)
	{
	    {
		    std::lock_guard<std::mutex> lck(_mutex);
			_queue.push_front(std::move(msg));
		}
	    _cond.notify_one();
	}
	T receive()
	{
	    std::unique_lock<std::mutex> lck(_mutex);
		_cond.wait(lck, [this]{ return !_queue.empty(); });
		T msg = std::move(_queue.back());
		_queue.pop_back();
		return msg;
	}
}

void listDirServer(MsgQueue<path> & dirQueue, MsgQueue<std::string> & fileQueue)
{
    for (;;)
	{
	    path dir = dirQueue.receive();
		 
		for (directory_iterator it(dir); it != directory_iterator(); ++it)
        {
	        if (it->is_directory())
		   {
		       dirQueue.send(it->path());
		   }
		   else
		   {
		       fileQueue.send(it->path().leaf());
		   }
		}
	}
}

void printServer(MsgQueue<std::string> & nameQueue)
{
    for(;;)
	{
}

void listTree(path && rootDir)
{
    MsgQueue<path> dirQueue;
    MsgQueue<std::string> fileQueue;
    dirQueue.send(std::move(rootDir));

    std::vector<std::future<void>> futures;
    for (int i=0; i < NUM_THREADS; ++i)
    {
        futures.push_back(std::async(std::launch::a	
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