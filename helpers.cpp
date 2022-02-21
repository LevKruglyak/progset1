#include "helpers.hpp"
#include <cassert>
#include <cctype>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <random>
#include <unistd.h>

/// Returns true if input represents an integer
bool is_integer_string(const char *input) {
	char *last;
	(void)strtol(input, &last, 10);
	return last != input && !*last && !isspace((unsigned char)*input);
}

/// Returns true if input only consists of string characters
bool is_real_string(const char *input) {
	char *last;
	(void)strtod(input, &last);
	return last != input && !*last && !isspace((unsigned char)*input);
}

/// Cryptographic hash function for unsigned integers.
uint32_t hash_1(uint32_t input) {
	// http://burtleburtle.net/bob/hash/integer.html
	input = (input ^ 61) ^ (input >> 16);
	input = input + (input << 3);
	input = input ^ (input >> 4);
	input = input * 0x27d4eb2d;
	input = input ^ (input >> 15);
	return input;
}

/// Return a random float in the [0,1] interval
float random_interval(uint32_t input) {
	// Based on this article about floating point hacking
	// https://www.iquilezles.org/www/articles/sfrand/sfrand.htm
	float res;
	*((uint32_t *)&res) = (hash_1(input) >> 9) | 0x3f800000;
	return res - 1.0f;
}
