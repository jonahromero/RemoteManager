#pragma once
#include <string>
#include <fstream>
class Batch
{
private:
	std::ofstream batch;
	std::string name;
	bool destroy;
	const char* destroy_line = R"((goto) 2>nul & del "%~f0")";
public:
	Batch(std::string const& title, bool destroy);
	void operator<<(std::string const&);
	void run();
	static std::string formatBatchPath(std::string str);
};

