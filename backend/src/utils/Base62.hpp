#pragma once
#include <cstdint>
#include <string>

namespace util {
	
std::string toBase62(uint64_t value);

uint64_t fromBase62(const std::string &code);
}
