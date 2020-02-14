#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
// queue size must be power of 2 as per spdlog docs
#define TRACE_FILE "trace.log"
#define INITIALIZE_BASIC_LOGGERS(qsize) \
spdlog::set_async_mode(qsize); \
spdlog::set_pattern("%Y/%m/%d %H:%M:%S.%e - %v"); \
spdlog::stdout_logger_st("console"); \
spdlog::basic_logger_st("debuglog", TRACE_FILE); \
spdlog::basic_logger_st("trclog", TRACE_FILE); \
spdlog::basic_logger_st("errlog", TRACE_FILE); \

#define console spdlog::get("console")->info
#define debuglog spdlog::get("debuglog")->debug
#define trclog spdlog::get("trclog")->info
#define errlog spdlog::get("errlog")->error