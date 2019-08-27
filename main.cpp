#include <iostream>
#include "bitcoin.h"
#include "input-utility.h"

using namespace bitcoin;

int main()
{
	std::vector<std::string> v;	// Receives input
	std::vector<Bytes> hashObjects;	// Hexstrings converted to Byte objects
	input::multiLineInput(v);	// stdin - see `input-utility.h`
	
	// Set up the txid hashes vector. Input hexstrings must be converted
	// to `Bytes` type, which is a `std::vector<uint8_t>`. Note that Bitcoin
	// requires the endianness to be reversed before hashing the txids.
	for (auto& el : v) {
		Bytes b;
		hexstringToBytes(el, b);
		byteSwap(b);
		hashObjects.push_back(b);
	}

	// Display the txids in `hashObjects`. Not necessary.
	std::cout << "hashObjects:\n";
	for (auto& el : hashObjects)
		printHex(el);

	Bytes result;	// Container for the result

	// Compute the Merkle root
	merkleRoot(hashObjects, result);
	
	// Note that by convention, byte order is reversed.
	byteSwap(result);
	std::cout << "Merkle root:\n";
	printHex(result);	

	return 0;
}
