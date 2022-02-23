#pragma once

#include <chrono>
#include <cstdint>

struct Timer {
	std::chrono::high_resolution_clock::time_point start_time, end_time;

	void start();
	void stop();

	unsigned int count();
	float seconds();
};

static Timer global_timer;

bool is_integer_string(const char *input);
bool is_real_string(const char *input);

uint32_t hash_1(uint32_t input);
uint32_t hash_2(uint32_t input);

float random_interval_iterative();
float random_interval(uint32_t input);
