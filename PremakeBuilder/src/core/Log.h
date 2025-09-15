#pragma once
#include "Base.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

class Log
{
public:
	static void init();
	inline static Ref<spdlog::logger>& GetLogger() { return s_Logger; }
private:
	static Ref<spdlog::logger> s_Logger;
};

//CLIENT LOG MACROS
#define LOG_TRACE(...)     ::Log::GetLogger()->trace(__VA_ARGS__);
#define LOG_INFO(...)      ::Log::GetLogger()->info(__VA_ARGS__);
#define LOG_WARN(...)      ::Log::GetLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...)     ::Log::GetLogger()->error(__VA_ARGS__);
#define LOG_FATAL(...)     ::Log::GetLogger()->fatal(__VA_ARGS__);