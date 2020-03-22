#include "HaffmanAlgoritm.h"

using std::cin;
using std::cout;
using std::endl;

int main()
{
	std::string sample;
	cout << "Enter the string to encode: ";
	std::getline(cin, sample);

	HuffmanTree encoding;
	auto code = EncodeToHuffmanCode(sample, encoding);
	auto decode = DecodeHuffmanCode(code, encoding);

	cout << "\nMemory usage: " << sample.length() * 8 << " bits" << endl;

	cout << "\nCodes and frequencies table:" << endl;
	PrintFrequencyAndCodeTable(encoding, sample);
	
	cout << "\nEncoding result: " << code << endl;
	cout << "Memory usage: " << trim(code).length() << " bits" << endl;

	cout << "\nCompression coefficient: " << CompressionCoefficient(sample, code) << endl;

	cout << "\nDecoding result: " << decode << endl;

	return 0;
}