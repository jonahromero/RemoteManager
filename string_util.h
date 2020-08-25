#pragma once
#include <string>
#include <vector>

auto repeat(std::string const& str, int times) -> std::string;
auto startsWith(std::string const& str, std::string const& begin) -> bool;
auto find_longest_str(std::vector<std::string> const& options) ->std::vector<std::string>::const_iterator;
auto isNumber(const std::string& s) -> bool;
auto bracket_split(std::string target, char delim = ' ')->std::vector<std::string>;

void append(std::string& base, char a);
void append(std::string& base, std::string a);
void append(std::string& base, const char * a);
template<typename...Args, 
	typename = std::enable_if_t<
	((std::is_same_v<Args, std::string> || 
	  std::is_same_v<Args, char> || 
	  std::is_same_v<Args, const char*>) && ...)>>
	auto strBuilder(std::string& base, Args...args) -> std::string& {
	(append(base, args), ...);
	return base;
}