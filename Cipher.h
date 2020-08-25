#pragma once
#include <string>

class CipherContract {
	virtual std::string encrypt_contract(std::string msg) = 0;
	virtual std::string decrypt_contract(std::string encrypted_msg) = 0;
};

template<typename T>
class Cipher 
	: public CipherContract
{
	std::string encrypt_contract(std::string msg) {
		return T::encrypt(msg);
	}
	std::string decrypt_contract(std::string encrypted_msg) {
		return T::decrypt(encrypted_msg);
	}
};

