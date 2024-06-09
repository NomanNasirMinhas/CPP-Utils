#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "rc4.h"


void printStringAsCharArray(const std::string& str) {
	std::cout << "char payload[] = {";
	for (size_t i = 0; i < str.size(); ++i) {
		std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(str[i]) & 0xff);
		if (i < str.size() - 1) {
			std::cout << ", ";
		}
	}
	std::cout << "};" << std::endl;
}
// Function to convert a string to hexadecimal Unicode characters
std::string stringToHexUnicode(const std::string& input) {
	std::string hexResult; // Resulting hexadecimal Unicode characters

	// Iterate over each character in the input string
	for (char ch : input) {
		// Convert the character to its Unicode code point
		uint32_t codePoint = static_cast<uint32_t>(ch);

		// Convert the code point to hexadecimal format
		std::stringstream ss;
		ss << std::hex << std::setw(4) << std::setfill('0') << codePoint;

		// Append the hexadecimal representation to the result
		hexResult += ss.str();
	}

	return hexResult; // Return the resulting hexadecimal Unicode characters
}

std::string invertHexUnicode(const std::string& input) {
	// start from the end of the string and take 2 characters at a time and print them in reverse order
	std::string invertedHexUnicode;
	for (int i = input.size() - 2; i >= 0; i -= 2) {
		invertedHexUnicode += input[i];
		invertedHexUnicode += input[i + 1];
	}

	// Remove the last 2 characters from the string
	invertedHexUnicode.pop_back();
	invertedHexUnicode.pop_back();

	// if length of the invertedHexUnicode is less than 14 add 0s to the beginning to make it 14 characters long
	if (invertedHexUnicode.length() < 14) {
		int diff = 14 - invertedHexUnicode.length();
		for (int i = 0; i < diff; i++) {
			invertedHexUnicode = "0" + invertedHexUnicode;
		}
	}


	return "0x00" + invertedHexUnicode;
}


void HexUnicodeToString(uint64_t hexValue) {
	// Hexadecimal value representing Unicode characters
	//uint64_t hexValue = 0x0061005600200079;

	// Convert the hexadecimal value to a wide string
	std::wstring wideStr;
	for (int i = 0; i < sizeof(hexValue) / sizeof(wchar_t); ++i) {
		wideStr += static_cast<wchar_t>((hexValue >> (i * 16)) & 0xFFFF);
	}
	// Print the wide string
	std::wcout << "'" << wideStr << "'" << " , Length: " << wideStr.length() << std::endl;
}

int main()
{
	int option;
	std::cout << "Select an option you want to perform. Select 0 to exit.\n";
	std::cout << "1. Convert a string to hexadecimal Unicode characters\n";
	std::cout << "2. Convert a hexadecimal value to a wide string\n";
	std::cout << "3. Encrypt with RC4\n";
	std::cout << "4. Decrypt with RC4\n";

	std::cin >> option;

	// Clear the input buffer

	if (option == 1) {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Enter a string to convert to hexadecimal Unicode characters: ";
		std::string input;
		std::getline(std::cin, input);

		std::cout << "Input string: " << input << std::endl;
		std::vector<std::string> substrings;
		for (size_t i = 0; i < input.length(); i += 4) {
			substrings.push_back(input.substr(i, 4));
		}

		for (size_t i = 0; i < substrings.size(); i++) {
			//std::cout << substrings[i] << std::endl;
			std::cout << "Token string: " << substrings[i] << " -> Length: " << substrings[i].length() << std::endl;
			std::string hexUnicode = stringToHexUnicode(substrings[i]); // Convert string to hexadecimal Unicode characters
			// Print the result
			//std::cout << "Original Inverted Hexadecimal Unicode characters: " << hexUnicode << std::endl;
			std::cout << "Hexadecimal: " << hexUnicode << " -> " << invertHexUnicode(hexUnicode) << std::endl;
		}

	}
	else if (option == 2) {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Enter a hexadecimal value to convert to a wide string in 0x0061... format: ";
		uint64_t hexValue;
		std::cin >> std::hex >> hexValue;
		HexUnicodeToString(hexValue);
	}
	else if (option == 3) {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		int sub_option;
		std::cout << "\n1. Encrypt String\n";
		std::cout << "2. Encrypt File\n";
		std::cin >> sub_option;
		if (sub_option == 1) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter the key for encryption: ";
			std::string key;
			std::getline(std::cin, key);
			RC4 rc4(key);
			std::cout << "Enter the string to encrypt: ";
			std::string data;
			std::getline(std::cin, data);
			std::string encryptedData = rc4.encrypt(data);
			std::cout << "Encrypted data\n "<< std::endl;
			printStringAsCharArray(encryptedData);
			
		}
		else if (sub_option == 2) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter the key for encryption: ";
			std::string key;
			std::getline(std::cin, key);
			RC4 rc4(key);
			std::cout << "Enter the input file path: ";
			std::string inputFilePath;
			std::getline(std::cin, inputFilePath);
			std::cout << "Enter the output file path: ";
			std::string outputFilePath;
			std::getline(std::cin, outputFilePath);
			rc4.encryptFile(inputFilePath, outputFilePath);
			std::cout << "File encrypted successfully\n";
		}
		else {
			std::cout << "Invalid option selected\n";
		}
	}
	else if (option == 4) {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		int sub_option;
		std::cout << "\n1. Decrypt String\n";
		std::cout << "2. Decrypt File\n";
		std::cin >> sub_option;
		if (sub_option == 1) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter the key for decryption: ";
			std::string key;
			std::getline(std::cin, key);
			RC4 rc4(key);
			std::cout << "Enter the string to decrypt: ";
			std::string data;
			std::getline(std::cin, data);
			std::string decryptedData = rc4.decrypt(data);
			std::cout << "Decrypted data: " << decryptedData << std::endl;
		}
		else if (sub_option == 2) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Enter the key for decryption: ";
			std::string key;
			std::getline(std::cin, key);
			RC4 rc4(key);
			std::cout << "Enter the input file path: ";
			std::string inputFilePath;
			std::getline(std::cin, inputFilePath);
			std::cout << "Enter the output file path: ";
			std::string outputFilePath;
			std::getline(std::cin, outputFilePath);
			rc4.decryptFile(inputFilePath, outputFilePath);
			std::cout << "File decrypted successfully\n";
		}
		else {
			std::cout << "Invalid option selected\n";
		}
	}

	else {
		std::cout << "Invalid option selected\n";
	}
	return 0;
}