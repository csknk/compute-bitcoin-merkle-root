#ifndef UTILITIES_H
#define UTILITIES_H
#include <vector>
#include <array>
#include <openssl/sha.h>
#include <iomanip>
#include <iostream>
#include <cassert>
#include <fstream>

namespace bitcoin {

// Main hash container size.
static constexpr size_t hash_size = 32;

// `std::vector` for variable length bytes container
typedef std::vector<uint8_t> Bytes; 

void writeLog(const std::string &text);
std::string bytesToHexstring(const Bytes& b);
int sha256(const uint8_t preimageBytes[], size_t len, Bytes& res);
int doubleSHA256(const uint8_t preimageBytes[], size_t len, Bytes& res);
int hexDigitToInt(char digit);
int hexstringToBytes(std::string const& hexstring, Bytes& result);
int hexstringToBytes(std::string const& hexstring, Bytes& result);
void merkleRoot(std::vector<Bytes> txids, Bytes& result);
void byteSwap(Bytes& b);
std::string bytesToHexstring(const Bytes& b);

template <typename T>
void printHex(T input)
{
	for (auto& el : input)
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)el;
	std::cout << '\n';
}

} // bitcoin

#endif
