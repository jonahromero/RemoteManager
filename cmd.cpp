#include "cmd.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <Windows.h>
#include <array>
#include "string_util.h"
#include "simplified_io.h"
#define R "\r"
#define NL "\n"
#define DELETE_LINE R, repeat(" ", current_msg.size() + cursor.size())

std::string const& Cmd::msg() const
{
    return current_msg;
}

void Cmd::write(std::string const& msg)
{
    std::string result;
    strBuilder(result, DELETE_LINE, R, msg, NL, cursor, current_msg);
    print(result);
}

Cmd::Cmd(std::string_view cursor) : cursor(cursor) {}

void Cmd::start_messaging()
{
    print(cursor);
}

void Cmd::backspace()
{
    if (!current_msg.empty()) {
        std::string wtp; //what to print
        if (current_index == current_msg.size()) {
            current_msg.pop_back();
            current_index--;
            strBuilder(wtp, R, repeat(" ", current_msg.size() + cursor.size() + 1), R, cursor, current_msg);
        }
        else if(current_index > 0) {
            current_index--;
            current_msg.erase(current_index, 1);
            strBuilder(wtp, R, repeat(" ", current_msg.size() + cursor.size() + 1), R, cursor, current_msg, R, cursor, current_msg.substr(0, current_index));
        }
        print(wtp);
    }
}

void Cmd::type(char letter)
{
    auto input = std::string(1, letter);
    if (current_index == current_msg.size()) {
        current_msg.push_back(letter);
        print(input);
    }
    else {
        current_msg.insert(current_index, input);
        std::string result;
        strBuilder(result, R, cursor, current_msg, repeat("\b", current_msg.size() - 1 - current_index));
        print(result);
    }
    current_index++;
}

void Cmd::move_left()
{
    if (current_index > 0) {
        current_index--;
        print("\b");
    }
}

void Cmd::move_right()
{
    if (current_index < current_msg.size()) {
        current_index++;
        std::string result;
        strBuilder(result, R, cursor, current_msg.substr(0, current_index));
        print(result);
    }
}

void Cmd::end_line(std::string const& prefix)
{
    std::string result;
    strBuilder(result, DELETE_LINE, R, prefix, current_msg, NL, cursor);
    print(result);
    current_msg = "";
    current_index = 0;
}

std::string Cmd::pipe_call(std::string const& cmd) const {
    std::array<char, 128> buffer;
    std::string result, cmd_err;
    if (dir.has_value()) {
        cmd_err = "cd " + dir.value() + " && ";
    }
    cmd_err.append(cmd + " 2>&1");
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd_err.c_str(), "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    if (*result.cend() == '\n') result.pop_back();
    return result;
}

void Cmd::set_directory(std::string const& dir)
{
    this->dir = dir;
}

void Cmd::hide_window()
{
    auto handle = GetConsoleWindow();
    ShowWindow(handle, SW_HIDE);
}


/* ARROW PROTOTYPE
else if (i == ARROW_SIG) {
            last_was_arrow = true;
        }
        else if (last_was_arrow) {
            last_was_arrow = false;
            switch (i) {
            case 75: //left
                write(std::string(1, '\b'));
                break;
            case 77: //right
                i = 'r';
                break;
            }
        }
*/