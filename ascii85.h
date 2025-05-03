#ifndef ASCII85_H
#define ASCII85_H
#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>
const std::string alphabet;
bool isLittleEndian();
std::vector<char> encoder(std::vector<char> &words);
std::vector<char> decoder(std::vector<char> &result);

#endif 