#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REL/Relocation.h>

#include <memory>
#include <string>

#include <ShlObj_core.h>
#include <Windows.h>
#include <Psapi.h>
#undef cdecl // Workaround for Clang 14 CMake configure error.

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;
using namespace REL::literals;

namespace logger = SKSE::log;
namespace stl = SKSE::stl;