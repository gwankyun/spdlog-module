export module spdlog.location;
import std;
import spdlog;
import spdlog.util;

using spdlog::util::format_with_location;
using spdlog::util::from_source_location;

export namespace spdlog::location
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
} // namespace spdlog::location
