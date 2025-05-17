#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#ifndef DEBUG
#define DEBUG 0
#endif

// Version number
#include <version.hpp>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shellapi.h>
#include <chrono>
#include <functional>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <mutex>
#include <map>
#include <unordered_set>
#include <cassert>
#include <queue>
#include <bridge_remix_api.h>

#pragma warning(push)
#pragma warning(disable: 26495)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning(pop)

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#if DEBUG
	#define DEBUG_PRINT(_MSG, ...) if constexpr (DEBUG) { \
		auto now = std::chrono::system_clock::now(); \
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000; \
		auto ts = std::format("{:%T}:{:03}", std::chrono::floor<std::chrono::seconds>(now), ms.count()); \
		OutputDebugStringA(utils::va("[%s] " _MSG, ts.c_str(), ##__VA_ARGS__)); }

	#define DEBUG_ASSERT(cond, msg, ...) if constexpr (DEBUG) { assert((cond) || !fprintf(stderr, (msg "\n"), ##__VA_ARGS__)); }
#else
	#define DEBUG_PRINT(_MSG, ...)
	#define DEBUG_ASSERT(cond, msg, ...)
#endif

#define ASSERT(cond, msg, ...) assert((cond) || !fprintf(stderr, (msg "\n"), ##__VA_ARGS__))
#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)
#define AssertSize(x, size)								static_assert(sizeof(x) == size, STRINGIZE(x) " structure has an invalid size.")
#define STATIC_ASSERT_SIZE(struct, size)				static_assert(sizeof(struct) == size, "Size check")
#define STATIC_ASSERT_OFFSET(struct, member, offset)	static_assert(offsetof(struct, member) == offset, "Offset check")

#include "game/structs.hpp"
#include "game/dvars.hpp"
#include "utils/utils.hpp"
#include "utils/memory.hpp"
#include "utils/hooking.hpp"
#include "utils/function.hpp"
#include "game/functions.hpp"

#include "components/fonts.hpp"
#include "components/loader.hpp"

using namespace std::literals;
