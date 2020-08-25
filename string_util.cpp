#include "string_util.h"
#include <cctype>
#include <optional>

std::string repeat(std::string const& str, int times) {
	std::string result;
	for (int i = 0; i < times; i++) result.append(str);
	return result;
}

bool startsWith(std::string const& str, std::string const& begin) {
	if (str.size() < begin.size()) return false;
	for (int i = 0; i < begin.size(); i++)
		if (str[i] != begin[i]) return false;
	return true;
}

std::vector<std::string>::const_iterator find_longest_str(std::vector<std::string> const& options) {
    int llen = 0, llen_index = 0;
    for (int i = 0; i < options.size(); i++) {
        if (options[i].size() > llen) {
            llen = options[i].size();
            llen_index = i;
        }
    }
    return options.begin() + llen_index;
}

bool isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void append(std::string& base, char a)
{
    base.push_back(a);
}

void append(std::string& base, std::string a)
{
    base.append(a);
}

void append(std::string& base, const char* a)
{
    base.append(a);
}

std::vector<std::string> bracket_split(std::string target, char delim) {
	std::vector<std::string> result;
	int in_bracket = 0;
	std::string token;
	std::optional<int> length_ident;
	for (int i = 0; i < target.size(); i++) {
		char& letter = target[i];
		//std::cout<<letter<<" In bracket: "<<in_bracket <<" Token:"<<token<<std::endl;
		if (letter == '[') {
			if (isNumber(token)) {
				length_ident = std::stoi(token);
				result.push_back(target.substr(i + 1, length_ident.value()));
				i += length_ident.value() + 2;
				length_ident.reset();
				token = "";
				continue;
			}
			in_bracket++;
		}
		else if (letter == ']') {
			in_bracket--;
		}
		else if (letter == delim && in_bracket == 0) {
			result.push_back(token);
			length_ident.reset();
			token = "";
			continue;
		}

		token.push_back(letter);
	}

	if (token != "") result.push_back(token);
	return result;
}
