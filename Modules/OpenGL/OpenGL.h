#pragma once

#include <string_view>

#ifdef OpenGL_EXPORTS
#define OpenGL_API __declspec(dllexport)
#else
#define OpenGL_API __declspec(dllimport)
#endif

const std::string_view LogOpenGL = "LogOpenGL";