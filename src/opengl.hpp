#pragma once
#include "call.hpp"

/// https://indiegamedev.net/2020/01/17/c-opengl-function-call-wrapping/

bool check_gl_errors(const std::string& filename, uint32_t line);

#define glCall_(function) callImpl(__FILE__, __LINE__, check_gl_errors, function)
#define glCall(function, ...) callImpl(__FILE__, __LINE__, check_gl_errors, function, __VA_ARGS__)
