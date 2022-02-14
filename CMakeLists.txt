cmake_minimum_required (VERSION 2.8.11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(randmst helpers.cpp main.cpp)
target_compile_features(randmst PUBLIC cxx_std_17)

