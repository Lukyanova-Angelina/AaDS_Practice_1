#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>

const std::string alphabet = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstu";

bool isLittleEndian() {
	uint32_t num = 0x1;
	uint8_t* ptr = reinterpret_cast<uint8_t*>(&num);
	return (ptr[0] == 0x1);
}

std::vector<char> encoder(std::vector<char> &words){
	int i = 0;
	std::vector<uint8_t> fourbytesdata;
	std::vector<char> result;
	while (i < words.size()) {
		uint32_t value = 0;
		size_t count = 0;
		for (int count = 0; count < 4 && i < words.size(); ++count, ++i) {
			fourbytesdata.push_back(static_cast<uint8_t>(words[i]));
		}
		while (fourbytesdata.size() < 4) {
			fourbytesdata.push_back(0x00);
		}
		if (!isLittleEndian()){
			value = fourbytesdata[0] | (fourbytesdata[1] << 8) | (fourbytesdata[2] << 16) | (fourbytesdata[3] << 24);
			
		} else{
			value = (fourbytesdata[0] << 24) | (fourbytesdata[1] << 16) | (fourbytesdata[2] << 8) | fourbytesdata[3];
		}
		fourbytesdata.clear();
		if (value == 0) {
			result.push_back('z');
		}else {
			uint8_t indices[5];
			uint32_t num = value;
			for (int j = 0; j < 5; ++j) {
				indices[j] = num % 85;
				num /= 85;
			}
			for (int j = 4; j >= 0; --j) {
				result.push_back(alphabet[indices[j]]);
			}
		}
	}
	if ((words.size() % 4) != 0) {
		size_t paddingCount = 5 - ((words.size() % 4) * 5 / 4);
		if (paddingCount > 0) {
			result.resize(result.size() - paddingCount + 1);
		}
	}

	return result;
}
std::vector<char> decoder(std::vector<char> &result) {
	std::vector<char> input;
	bool lendian = isLittleEndian();
	std::vector<char> words;
	size_t i = 0;

	for (char c : result) {
		if (c == 'z') {
			for (int i = 0; i < 5; ++i) input.push_back('!');
		} else if (c != ' ' && c != '\n' && c != '\r') {
			input.push_back(c);
		}
	}
	result = input;
	size_t padding = (5 - result.size() % 5) % 5;
	for (size_t i = 0; i < padding; ++i) result.push_back('u');

	while (i < result.size()) {
		uint32_t num = 0;
		int count = 0;
		for (; count < 5 && i < result.size(); ++count, ++i) {
			size_t pos = alphabet.find(result[i]);
			num = num * 85 + static_cast<uint32_t>(pos);
		}

		if (count < 5) {
			num *= 85UL * 85UL * 85UL * 85UL;
		}

		uint8_t byte0 = (num >> 24) & 0xFF;
		uint8_t byte1 = (num >> 16) & 0xFF;
		uint8_t byte2 = (num >> 8)  & 0xFF;
		uint8_t byte3 = num & 0xFF;

		if (!lendian) {
			words.push_back(byte3);
			words.push_back(byte2);
			words.push_back(byte1);
			words.push_back(byte0);
		} else {
			words.push_back(byte0);
			words.push_back(byte1);
			words.push_back(byte2);
			words.push_back(byte3);
		}
	}

	if (padding > 0 && !words.empty()) {
		words.resize(words.size() - padding);
	}

	return words;
}

int main(int argc, char* argv[]) {
	bool decode_mode = false;

	if (argc > 1) {
		std::string flag(argv[1]);
		if (flag == "-d") decode_mode = true;
		else if (flag != "-e") {
			std::cerr << "Usage: " << argv[0] << " [-e | -d]\n";
			return 1;
		}
	}

	try {

		if (decode_mode) {
			std::vector<char> input(
				std::istreambuf_iterator<char>(std::cin),
				std::istreambuf_iterator<char>()
			);
			std::vector<char> decoded = decoder(input);
			std::copy(decoded.begin(), decoded.end(),
				std::ostreambuf_iterator<char>(std::cout));
		} else {
			std::vector<char> input(
				std::istreambuf_iterator<char>(std::cin),
				std::istreambuf_iterator<char>()
			);
			std::vector<char> encoded = encoder(input);
			std::copy(encoded.begin(), encoded.end(),
				std::ostreambuf_iterator<char>(std::cout));
		}
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}