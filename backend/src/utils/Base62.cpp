#include "Base62.hpp"

namespace {

constexpr char kAlphabet[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr int kBase = 62;

}

namespace util {

std::string toBase62(uint64_t value) {

	if(value == 0) {
		return "0";
	}

	std::string out;

	while (value > 0) {
		out += kAlphabet[value % kBase];
		value /= kBase;
	}

	return {out.rbegin(), out.rend()};
}

uint64_t fromBase62(const std::string &code) {

	uint64_t value = 0;

	for(auto c : code) {
		int digit{};

		if(c >= '0' && c <= '9') {
			digit = c - '0';
		} else if (c >= 'a' && c <= 'z') {
			digit = c - 'a' + 10;
		} else if (c >= 'A' && c <= 'Z') {
			digit = c - 'A' + 36;
		} else {
			return 0;
		}

		value = value * kBase + static_cast<uint64_t>(digit);
	}

	return value;
}
}
