#pragma once

#include <assert.hpp>
#include <inplace_function.hpp>
#include <mutex.hpp>
#include <prohibit_copy_move.hpp>
#include <singleton.hpp>

#include <array>
#include <ctime>
#include <experimental/source_location>
#include <string_view>

namespace logger
{

static constexpr auto MAX_MESSAGE_LENGTH = 160;
static constexpr auto MAX_HEADER_LENGTH = 80;
static constexpr auto ENTRIES_DELIMITER = "\n";

using SourceLoc = std::experimental::source_location;
using get_timestamp_cb_t = common::InplaceFunction<std::time_t()>;

enum class Severity : uint8_t
{
    FATAL = 0,
    ERROR,
    WARN,
    INFO,
    DEBUG
};

class Logger
{
public:
    Logger(
        Severity sev = Severity::INFO, get_timestamp_cb_t get_timestamp_cb = [] { return std::time(nullptr); })
        : m_sev(sev), m_get_timestamp_cb(std::move(get_timestamp_cb))
    {}

    template <typename... TArgs>
    void log(SourceLoc loc, Severity sev, std::string_view format, TArgs&&... args);

private:
    Severity m_sev;
    get_timestamp_cb_t m_get_timestamp_cb;

    std::array<char, MAX_HEADER_LENGTH> m_entry_header;
    std::array<char, MAX_MESSAGE_LENGTH> m_entry_body;

    freertos::Mutex m_mutex;
};

template <typename... TArgs>
void Logger::log(SourceLoc loc, Severity sev, std::string_view format, TArgs&&... args)
{
    auto ts = m_get_timestamp_cb();
    if (m_sev < sev) return;

    std::lock_guard lock(m_mutex);

    constexpr auto to_string = [](Severity sev) -> std::string_view {
        switch (sev) {
            case Severity::FATAL: return "F";
            case Severity::ERROR: return "E";
            case Severity::WARN: return "W";
            case Severity::INFO: return "I";
            case Severity::DEBUG: return "D";
        }
        ASSERT(false);
        return {};
    };

    const std::string_view file_path = loc.file_name();
    // TODO: add milliseconds
    const size_t offset = std::strftime(m_entry_header.data(), 30, "%Y-%m-%d-%H:%M:%S", std::localtime(&ts));

    std::snprintf(m_entry_header.data() + offset, m_entry_header.max_size() - offset, " [%s] [%s:%u] ",
                  to_string(sev).data(), file_path.substr(file_path.find_last_of("/") + 1).data(), loc.line());

    std::snprintf(m_entry_body.data(), m_entry_body.max_size(), format.data(), std::forward<TArgs>(args)...);

    std::printf("%s%s%s", m_entry_header.data(), m_entry_body.data(), ENTRIES_DELIMITER);
}

using SingleLogger = common::Singleton<Logger>;

template <typename... TArgs>
inline void log(SourceLoc loc, Severity sev, std::string_view format, TArgs&&... args)
{
    SingleLogger::Ptr logger = SingleLogger::instance();
    ASSERT(logger);

    logger->log(loc, sev, format, std::forward<TArgs>(args)...);
}

}  // namespace logger

#include <logger_macros.hpp>
