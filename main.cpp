#include <iostream>
#include "bitcoin.h"
#include "input-utility.h"

using namespace bitcoin;

int main()
{
	std::vector<std::string> v;	// Receives input
	std::vector<Bytes> hashObjects;	// Hexstrings converted to Byte objects
	input::multiLineInput(v);
	for (auto& el : v) {
		Bytes b;
		hexstringToBytes(el, b);
		byteSwap(b);
		hashObjects.push_back(b);
	}

	std::cout << "hashObjects:\n";
	for (auto& el : hashObjects)
		printHex(el);

	// Returns a v large vector	
	// Bytes m = merkleRoot(hashObjects);
	Bytes result; 
	merkleRoot(hashObjects, result);
	std::cout << "Merkle root:\n";
	byteSwap(result);
	printHex(result);	

	return 0;
}
