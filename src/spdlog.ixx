module;

#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>

export module spdlog;
import std;

std::string_view extract_function_name(std::string_view function_name)
{
    // 1. 找到最后一个 ')'
    if (function_name.empty())
    {
        return function_name;
    }
    size_t end_pos = function_name.rfind(')');
    if (end_pos == std::string_view::npos)
    {
        return function_name;
    }

    // 2. 找到与末尾 ')' 匹配的 '('
    int bracket_count = 1;
    size_t open_paren_pos = end_pos - 1;
    while (open_paren_pos > 0 && bracket_count > 0)
    {
        if (function_name[open_paren_pos] == ')')
        {
            ++bracket_count;
        }
        else if (function_name[open_paren_pos] == '(')
        {
            --bracket_count;
        }
        --open_paren_pos;
    }
    if (bracket_count > 0)
    {
        return function_name;
    }

    // 修正open_paren_pos，使其指向'('的位置
    ++open_paren_pos;

    // 3. 找到 '(' 前面最近的空格
    size_t last_space_pos = open_paren_pos;
    while (last_space_pos > 0 && function_name[last_space_pos - 1] != ' ')
    {
        --last_space_pos;
    }

    // 4. 返回空格和这个 '(' 之间的字符
    return function_name.substr(last_space_pos,
                                open_paren_pos - last_space_pos);
}

spdlog::source_loc from_source_location(
    std::source_location _location = std::source_location::current())
{
    auto file_name = _location.file_name();
    auto line = _location.line();
    auto function_name = _location.function_name();

    // 提取函数名
    thread_local std::array<char, 256> func_name;
    auto name_view = extract_function_name(std::string_view(function_name));
    size_t copy_len = std::min(name_view.length(), static_cast<size_t>(255));
    std::copy_n(name_view.begin(), copy_len, func_name.data());
    func_name[copy_len] = '\0';

    // 运行时动态生成source_loc
    return spdlog::source_loc(file_name, line, func_name.data());
}

class Logger
{
  public:
    Logger(
        spdlog::logger* _logger = spdlog::default_logger_raw(),
        const std::source_location _location = std::source_location::current())
        : logger(_logger), location(_location)
    {
    }
    ~Logger() = default;

    template <typename... Args>
    void log(spdlog::level::level_enum _level,
             const std::format_string<Args...> fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.get(), std::make_format_args(args...));
        spdlog::source_loc loc(from_source_location(location));
        logger->log(loc, _level, message);
    }

    template <typename... Args>
    void trace(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::trace, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::info, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::err, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void critical(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }

  private:
    std::source_location location;
    spdlog::logger* logger = spdlog::default_logger_raw();
};

namespace logging
{
    using source_location = std::source_location;

    struct format_with_location
    {
        std::string_view value;
        std::source_location loc;

        template <typename String>
        format_with_location(const String& s, const source_location& location =
                                                  source_location::current())
            : value{s}, loc{location}
        {
        }
    };

    void log(spdlog::logger* logger, std::source_location loc,
             spdlog::level::level_enum level, const std::string& message)
    {
        logger->log(from_source_location(loc), level, message);
    }

    void log(std::source_location loc, spdlog::level::level_enum level,
             const std::string& message)
    {
        log(spdlog::default_logger_raw(), loc, level, message);
    }

    template <typename... Args>
    void trace(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.loc, spdlog::level::trace, message);
    }

    template <typename... Args>
    void debug(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.loc, spdlog::level::debug, message);
    }

    template <typename... Args>
    void info(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.loc, spdlog::level::info, message);
    }

    template <typename... Args>
    void warn(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.loc, spdlog::level::warn, message);
    }

    template <typename... Args>
    void error(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.loc, spdlog::level::err, message);
    }

    template <typename... Args>
    void critical(format_with_location fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.value, std::make_format_args(args...));
        log(fmt.loc, spdlog::level::critical, message);
    }
} // namespace logging

export namespace spdlog
{
    Logger get(
        spdlog::logger* logger = spdlog::default_logger_raw(),
        const std::source_location location = std::source_location::current())
    {
        Logger current(logger, location);
        return current;
    }

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
    using spdlog::set_level;
    using spdlog::set_pattern;
    using spdlog::to_hex;
} // namespace spdlog
