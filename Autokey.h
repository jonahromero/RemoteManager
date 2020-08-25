#pragma once
#include "Cipher.h"
class Autokey :
    public Cipher<Autokey>
{
public:
	static std::string encrypt(std::string msg);
	static std::string decrypt(std::string encrypted_msg);
};

