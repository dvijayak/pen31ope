#include <iostream>
#include <vector>
#include <spdlog/spdlog.h>
#include "spdlog/async.h"
#include <spdlog/fmt/ostr.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
// queue size must be power of 2 as per spdlog docs
#define TRACE_FILE "trace.log"
#define INITIALIZE_BASIC_LOGGERS() \
spdlog::init_thread_pool(8192, 1); \
auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >(); \
auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(TRACE_FILE, true); \
std::vector<spdlog::sink_ptr> sinks{ stdout_sink, file_sink }; \
auto logger = std::make_shared<spdlog::async_logger>("loggername", sinks.begin(), sinks.end(), spdlog::thread_pool(),  spdlog::async_overflow_policy::block); \
spdlog::register_logger(logger); \
spdlog::set_pattern("%Y/%m/%d %H:%M:%S.%e - %v");

// #define console spdlog::get("console")->info
// #define debuglog spdlog::get("debuglog")->debug
// #define trclog spdlog::get("trclog")->info
// #define errlog spdlog::get("errlog")->error

#define trclog std::cout << 