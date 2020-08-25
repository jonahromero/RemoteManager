#pragma once
#include <string>
#include <atomic>

//OUTPUT
void print(std::string const& message);
void print_loading(std::atomic<bool>* done_flag); //prints ascii loading symbol

//INPUT
char get(); //get char
int get_nb(); //get char (non-blocking). -1 on nothing
void wait_key(); //wait for key, do nothing with it
int getNumber();
