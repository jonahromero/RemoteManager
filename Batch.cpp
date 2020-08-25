#include "Batch.h"

Batch::Batch(std::string const& title, bool destroy) : destroy(destroy), name(title)
{
	name.append(".bat");
	batch.open(name);
}

void Batch::operator<<(std::string const& str)
{
	if (batch.is_open()) {
		batch << str << std::endl;
	}
}

void Batch::run()
{
	if (batch.is_open()) {
		if (destroy) {
			batch << destroy_line;
		}
		batch.close();
		system(name.c_str());
	}
}

std::string Batch::formatBatchPath(std::string str)
{
	int lastIndex = 0;
	bool endQuoteNext = false;
	for (int i = 0; i < str.size(); i++) {
		char& letter = str[i];
		if (letter == '/' || letter == '\\') {
			if (endQuoteNext) {
				str.insert(i, "\"");
				return str;
			}
			lastIndex = i;
		}
		else if (letter == ' ') {
			endQuoteNext = true;
			str.insert(lastIndex + 1, "\"");
			i++;
		}
	}
	return str;
}
