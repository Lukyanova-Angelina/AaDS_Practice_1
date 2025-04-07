
#include "ascii85.h"
#include <gtest/gtest.h>
TEST(EncoderTest, EmptyInput) { // check empty input
	std::vector<char> input;
	std::vector<char> encoded = encoder(input);
	EXPECT_TRUE(encoded.empty());
}

TEST(EncoderTest, SingleZeroBlock) { // check if 0000 equals 'z'
	std::vector<char> input(4, 0);
	std::vector<char> encoded = encoder(input);
	ASSERT_EQ(encoded.size(), 1);
	EXPECT_EQ(encoded[0], 'z');
}

TEST(EncoderTest, PartialBlock) { // check one byte encoder
	std::vector<char> input{'A'};
	std::vector<char> encoded = encoder(input);
	ASSERT_GE(encoded.size(), 2);
	EXPECT_NE(encoded[0], 'z');
}

TEST(DecoderTest, SingleZBlock) { // check if z equals 0000
	std::vector<char> input{'z'};
	std::vector<char> decoded = decoder(input);
	ASSERT_EQ(decoded.size(), 4);
	for (char c : decoded) EXPECT_EQ(c, 0);
}

TEST(DecoderTest, WhitespaceHandling) { // check std::isspace(static_cast<unsigned char>(c))
	std::vector<char> input{' ', '\n', '\t', 'z'};
	std::vector<char> decoded = decoder(input);
	ASSERT_EQ(decoded.size(), 4);
	for (char c : decoded) EXPECT_EQ(c, 0);
}

TEST(DecoderTest, FullBlockDecode) { //  !!!!! equals z
	std::vector<char> input{'!', '!', '!', '!', '!'};
	std::vector<char> decoded = decoder(input);
	std::vector<char> encoded = encoder(decoded);
	ASSERT_EQ(decoded.size(), 4);
	ASSERT_EQ(encoded, std::vector<char>{'z'});
}

TEST(IntegrationTest, RoundTripSimple) { // convert %4 input
	std::string test_data = "test";
	std::vector<char> input(test_data.begin(), test_data.end());
	
	auto encoded = encoder(input);
	auto decoded = decoder(encoded);
	
	ASSERT_EQ(input, decoded);
}

TEST(IntegrationTest, RoundTripWithPadding) {// convert not %4 input
	std::string test_data = "hello";
	std::vector<char> input(test_data.begin(), test_data.end());
	auto encoded = encoder(input);
	auto decoded = decoder(encoded);
	input.resize(8);
	decoded.resize(5);
	input.resize(5);
	ASSERT_EQ(input, decoded);
}
