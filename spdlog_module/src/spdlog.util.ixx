module;

#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>

export module spdlog.util;
import std;

export namespace spdlog::util
{
    using source_location = std::source_location;

    template <typename T>
    concept StringLike = requires(T t) {
        { std::string_view{t} } -> std::same_as<std::string_view>;
    };

    struct format_with_location
    {
        std::string_view value;
        std::source_location location;

        template <StringLike String>
        format_with_location(
            const String& _s,
            const source_location& _location = source_location::current())
            : value{_s}, location{_location}
        {
        }
    };

    std::string_view extract_function_name(std::string_view _function_name)
    {
        auto func = _function_name;
        // 1. 找到最后一个 ')' ，用 e 表示
        if (func.empty())
        {
            return func;
        }
        auto end_pos = func.rfind(')');
        if (end_pos == std::string_view::npos)
        {
            return func;
        }

        // 2. 找到与 e 匹配的 '(' ，用 b 表示
        auto bracket_count = 1;
        auto open_it = func.rbegin() + (func.size() - end_pos);
        while (open_it != func.rend() // 找不到 '('
               && bracket_count > 0   // 數量不匹配
        )
        {
            if (*open_it == ')')
            {
                ++bracket_count;
            }
            else if (*open_it == '(')
            {
                --bracket_count;
            }
            ++open_it;
        }
        if (bracket_count > 0)
        {
            return func;
        }

        // 修正位置，使其指向 b 的位置
        auto open_paren_pos =
            func.size() - std::distance(func.rbegin(), open_it);

        // 3. 找到 b 前面最近的空格或者 ':'，用 s 表示
        auto last_space_pos = open_paren_pos;
        auto it = std::find_if(func.rbegin() + (func.size() - open_paren_pos),
                               func.rend(),
                               [](char c) { return c == ' ' || c == ':'; });
        if (it != func.rend())
        {
            last_space_pos = func.size() - std::distance(func.rbegin(), it);
        }

        // 4. 返回 [s, b) 區間的字符
        return func.substr(last_space_pos, open_paren_pos - last_space_pos);
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
        size_t copy_len =
            std::min(name_view.length(), static_cast<size_t>(255));
        std::copy_n(name_view.begin(), copy_len, func_name.data());
        func_name[copy_len] = '\0';

        // 运行时动态生成 source_loc
        return spdlog::source_loc(file_name, line, func_name.data());
    }
} // namespace spdlog::util
