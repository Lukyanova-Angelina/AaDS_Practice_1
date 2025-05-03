#include "ascii85.h"
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
