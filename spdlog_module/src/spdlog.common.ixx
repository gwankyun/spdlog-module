module;

#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>

export module spdlog.common;
import std;

export namespace spdlog
{
    namespace level
    {
        using spdlog::level::level_enum;
    }
    using spdlog::default_logger;
    using spdlog::default_logger_raw;
    using spdlog::logger;
    using spdlog::set_level;
    using spdlog::set_pattern;
    using spdlog::source_loc;
    using spdlog::to_hex;
} // namespace spdlog
