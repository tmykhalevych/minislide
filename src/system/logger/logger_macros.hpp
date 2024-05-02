#pragma once

// clang-format off

#define LOG(sev, format, ...) logger::log(logger::SourceLoc::current(), (sev), (format), ## __VA_ARGS__)

#define LOG_FATAL(format, ...) { LOG(logger::Severity::FATAL, (format), ## __VA_ARGS__); configASSERT(false); }
#define LOG_ERROR(format, ...) { LOG(logger::Severity::ERROR, (format), ## __VA_ARGS__); }
#define LOG_WARN(format, ...)  { LOG(logger::Severity::WARN,  (format), ## __VA_ARGS__); }
#define LOG_INFO(format, ...)  { LOG(logger::Severity::INFO,  (format), ## __VA_ARGS__); }
#define LOG_DEBUG(format, ...) { LOG(logger::Severity::DEBUG, (format), ## __VA_ARGS__); }

// clang-format on
