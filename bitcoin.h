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
// Common hash container sizes.
static constexpr size_t hash_size = 32;
static constexpr size_t half_hash_size = hash_size / 2;
static constexpr size_t quarter_hash_size = half_hash_size / 2;
static constexpr size_t long_hash_size = 2 * hash_size;
static constexpr size_t short_hash_size = 20;
static constexpr size_t mini_hash_size = 6;

// `std::array` container for `Size` bytes
template<size_t Size>
using byte_array = std::array<uint8_t, Size>;

// `std::array` holding `hash_size` bytes
typedef byte_array<hash_size> hash_digest;

// `sts::vector` for variable length bytes container
typedef std::vector<uint8_t> Bytes; 

void writeLog(const std::string &text);
std::string bytesToHexstring(const Bytes& b);


//int sha256(const uint8_t preimageBytes[], size_t len, Bytes& res)
int sha256(const uint8_t preimageBytes[], size_t len, Bytes& res)
{
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, preimageBytes, len); 
	int ret = SHA256_Final(res.data(), &sha256);
	return ret;
}

int doubleSHA256(const uint8_t preimageBytes[], size_t len, Bytes& res)
{
	sha256(preimageBytes, len, res);
	return sha256(res.data(), res.size(), res);
}

int hexDigitToInt(char digit)
{
	digit = tolower(digit);
	if (digit >= '0' && digit <='9')
	       return (int)(digit - '0');
	else if (digit >= 'a' && digit <= 'f') {
		return (int)(digit - '1' - '0') + 10; 
	}	
	return -1;
}

int hexstringToBytes(std::string const& hexstring, Bytes& result)
{
	if (hexstring.size() % 2) {
		std::cerr << "The hexstring is not an even number of characters.\n";
		exit(EXIT_FAILURE);
	}
	
	size_t resultLength = hexstring.size() / 2;
	size_t i = 0;
	for (auto it = hexstring.begin(); it != hexstring.end(); it += 2) {
		int sixteens = hexDigitToInt(*it);
		int units = hexDigitToInt(*std::next(it));
		result.push_back((sixteens << 4) | units);
		i++;
	}
	return resultLength;
}
	

template <typename T>
void printHex(T input)
{
	for (auto& el : input)
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)el;
	std::cout << '\n';
}

void merkleRoot(std::vector<Bytes> txids, Bytes& result)
{
	if (txids.empty()) {
		throw;
	}
	while (txids.size() > 1) {
		writeLog("while loop");
		// If odd number, add the last element to end of vector
		if (txids.size() & 1) {
			writeLog("duplicating last element.\n");
			txids.push_back(txids.back());
		}
		std::vector<Bytes> tmp;
		for (auto it = std::begin(txids); it != std::end(txids) && std::next(it) != txids.end(); it += 2) {
			Bytes concat = *it;
			Bytes result(hash_size);
			concat.insert(concat.end(), (*(it + 1)).begin(), (*(it + 1)).end());
			doubleSHA256(concat.data(), 64/*concat.size()*/, result);
			tmp.push_back(result);
			concat.clear();
		}
		std::ostringstream ss;
		ss << "Current hashes:\n";
		ss << "---------------\n";
		ss << txids.size() << "txids in this iteration.\n"; 
		for (auto& el : tmp)
			ss << bytesToHexstring(el) << '\n';
		writeLog(ss.str());
		txids = tmp;
	}
	result = txids[0];
}

void byteSwap(Bytes& b)
{
	Bytes tmp;
	for (auto i = b.size(); i-- > 0;) {
		tmp.push_back(b[i]);
	}
	b = tmp;
}

void writeLog(const std::string &text)
{
    std::ofstream log_file(
        "log_file.txt", std::ios_base::out | std::ios_base::app );
    log_file << text << std::endl;
}

std::string bytesToHexstring(const Bytes& b)
{
	std::ostringstream ss;
	for (auto& el : b) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)el;
	}
	return ss.str();
}

} // bitcoin

#endif
