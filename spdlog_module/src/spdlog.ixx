module;

#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>

export module spdlog;
import std;
import spdlog.util;
import spdlog.logger;

using spdlog::util::format_with_location;
using spdlog::util::from_source_location;

namespace logging
{
    void log(std::source_location location, spdlog::level::level_enum level,
             const std::string& message)
    {
        spdlog::default_logger_raw()->log(from_source_location(location), level,
                                          message);
    }

    template <typename... Args>
    void trace(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.location, spdlog::level::trace, message);
    }

    template <typename... Args>
    void debug(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.location, spdlog::level::debug, message);
    }

    template <typename... Args>
    void info(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.location, spdlog::level::info, message);
    }

    template <typename... Args>
    void warn(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.location, spdlog::level::warn, message);
    }

    template <typename... Args>
    void error(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.location, spdlog::level::err, message);
    }

    template <typename... Args>
    void critical(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.location, spdlog::level::critical, message);
    }
} // namespace logging

export namespace spdlog
{
    using logging::critical;
    using logging::debug;
    using logging::error;
    using logging::info;
    using logging::trace;
    using logging::warn;

    namespace level
    {
        using spdlog::level::level_enum;
    }
    using spdlog::default_logger_raw;
    using spdlog::default_logger;
    using spdlog::set_level;
    using spdlog::set_pattern;
    using spdlog::to_hex;
    using spdlog::logger;
} // namespace spdlog
