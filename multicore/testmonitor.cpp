
class MonitorResult
{
    Result _result;
	std::mutex _mutex;
public:
    void putFile(std::string && file)
	{
	    std::lock_guard<std::mutex> lck(_mutex);
		_result.files.push_back(file);
	}
    void putDir(path && pth)
	{
	    std::lock_guard<std::mutex> lck(_mutex);
		_result.files.push_back(pth);
	}
	std::vector<path> getDirs(int n)
	{
	    std::vector<path> dirs;
		std::lock_guard<std::mutex> lck(_mutex);
		
	}
};

int main()
{
    ResultMonitor result;
	for (directory_iterator it(path("c:\\windows\\")); it != directory_iterato
	    result.putDir(path("C:\\windows"));
	auto f1 = std::async(&listTree, std::ref(result));
	auto f2 = std::async(&listTree, std::ref(result));
	f1.wait();
	f2.wait();
}