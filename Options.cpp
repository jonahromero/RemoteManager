#include "Options.h"
#include <iostream>
#include <array>
#include <thread>
#include "string_util.h"
#include "simplified_io.h"

using namespace std::string_literals;

std::string make_option_header(std::vector<std::string> const& options) {
    auto longest = find_longest_str(options)->size();
    std::string result, top = "+--" + repeat("-", longest) + "+\n";
    result.append(top);
    for (int i = 0; i < options.size(); i++) {
        int spaces = longest - options[i].size();
        result.append("|" + std::to_string(i + 1) + "." + options[i] + repeat(" ", spaces) + "|\n");
    }
    return result.append(top);
}

Options::Options(std::vector<std::string> const& options) : options(options), funcs(options.size()) {}
Options::Options(std::vector<std::string>&& options) : options(options), funcs(options.size()) {}

void Options::chose(int input)
{
    if (input > 0 && input < options.size() + 1) {
        funcs[input - 1]();
    }
    else
       std::cout << "Invalid Input: Please Enter Valid Option" << std::endl;
}

void Options::ask()
{
    std::cout << make_option_header(options);

    int input = 0;
    while (input == 0) {
        std::cout << "Option:";
        input = getNumber();

        if (input > 0 && input < options.size() + 1) {
            funcs[input - 1]();
            system("CLS");
            ask();
            return;
        }
        std::cout << "Invalid Input: Please Enter Valid Option" << std::endl;
        input = 0;
    }
}

std::function<void()>& Options::operator[](int index)
{
    return funcs[index - 1];
}
