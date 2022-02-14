#include "helpers.hpp"
#include <random>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <cstring>
#include <cstdarg>
#include <unistd.h>

bool is_integer_string(const char* s) {
    char* last;
    (void) strtol(s, &last, 10);
    return last != s && !*last && !isspace((unsigned char) *s);
}

bool is_real_string(const char* s) {
    char* last;
    (void) strtod(s, &last);
    return last != s && !*last && !isspace((unsigned char) *s);
}

