#include "input-utility.h"

namespace input {
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

