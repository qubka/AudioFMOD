#pragma once
#include "call.hpp"

/// https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/

bool check_al_errors(const std::string& filename, uint32_t line);
bool check_alc_errors(const std::string& filename, uint32_t line);

#define alCall_(function) callImpl(__FILE__, __LINE__, check_al_errors, function)
#define alCall(function, ...) callImpl(__FILE__, __LINE__, check_al_errors, function, __VA_ARGS__)

#define alcCall_(function) callImpl(__FILE__, __LINE__, check_alc_errors, function)
#define alcCall(function, ...) callImpl(__FILE__, __LINE__, check_alc_errors, function, __VA_ARGS__)
