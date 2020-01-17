#include "input-utility.h"

namespace input {
/**
 * This function is ueful for supplying TXIDS from a multi-line
 * manifest file, building data into a vector of strings.
 * */
void multiLineInput(std::vector<std::string>& v)
{
	// Discard leading whitespace
	std::cin >> std::ws;

	std::string line;
	while(getline(std::cin, line) && !line.empty()) {
		v.push_back(line);
	}
}
} // input

