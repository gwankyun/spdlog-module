export module spdlog.location.logger;
import std;
import spdlog.common;
import spdlog.util;

using spdlog::util::format_with_location;
using spdlog::util::from_source_location;

namespace spdlog
{
    class Logger
    {
      public:
        Logger(
            std::shared_ptr<spdlog::logger> _logger = spdlog::default_logger())
            : logger(_logger)
        {
        }

        ~Logger() = default;

        void log(std::source_location location, spdlog::level::level_enum level,
                 const std::string& message)
        {
            logger->log(from_source_location(location), level, message);
        }

        template <typename... Args>
        void trace(format_with_location fmt, Args&&... args)
        {
            auto message =
                std::vformat(fmt.value, std::make_format_args(args...));
            log(fmt.location, spdlog::level::trace, message);
        }

        template <typename... Args>
        void debug(format_with_location fmt, Args&&... args)
        {
            auto message =
                std::vformat(fmt.value, std::make_format_args(args...));
            log(fmt.location, spdlog::level::debug, message);
        }

        template <typename... Args>
        void info(format_with_location fmt, Args&&... args)
        {
            auto message =
                std::vformat(fmt.value, std::make_format_args(args...));
            log(fmt.location, spdlog::level::info, message);
        }

        template <typename... Args>
        void warn(format_with_location fmt, Args&&... args)
        {
            auto message =
                std::vformat(fmt.value, std::make_format_args(args...));
            log(fmt.location, spdlog::level::warn, message);
        }

        template <typename... Args>
        void error(format_with_location fmt, Args&&... args)
        {
            auto message =
                std::vformat(fmt.value, std::make_format_args(args...));
            log(fmt.location, spdlog::level::err, message);
        }

        template <typename... Args>
        void critical(format_with_location fmt, Args&&... args)
        {
            auto message =
                std::vformat(fmt.value, std::make_format_args(args...));
            log(fmt.location, spdlog::level::critical, message);
        }

        std::shared_ptr<spdlog::logger> get()
        {
            return logger;
        }

      private:
        std::shared_ptr<spdlog::logger> logger;
    };

    export namespace location
    {
        using logger = Logger;
    }
} // namespace spdlog
