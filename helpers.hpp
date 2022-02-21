#pragma once

#include <cstdint>

bool is_integer_string(const char *input);
bool is_real_string(const char *input);

uint32_t hash_1(uint32_t input);

float random_interval(uint32_t input);
