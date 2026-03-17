#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

import std;
import spdlog;
import spdlog.util;
import spdlog.location.alias;

using function_info = std::unordered_map<std::string, std::string>;
function_info g_function_name;

void save_func(const char* _expect,
               function_info& _function_name = g_function_name,
               std::source_location _location = std::source_location::current())
{
    _function_name[_expect] =
        spdlog::util::extract_function_name(_location.function_name());
}

struct Object
{
    Object() = default;
    ~Object() = default;
    void f()
    {
        spdlog::info("__func__: {}", __func__);
        spdlog::info("function_name: {}",
                     std::source_location::current().function_name());
        save_func("f");
    }
};

namespace space
{
    void g()
    {
        spdlog::info("__func__: {}", __func__);
        spdlog::info("function_name: {}",
                     std::source_location::current().function_name());
        save_func("g");
    }
} // namespace space

TEST_CASE("extract_function_name", "[spdlog]")
{
    for (auto i : g_function_name)
    {
        REQUIRE(i.first == i.second);
    }
}

int main(int _argc, char* _argv[])
{
    spdlog::set_pattern("[%C-%m-%d %T.%e] [%^%L%$] [%-8!!:%4#] %v");

    std::print(std::source_location::current().function_name());

    spdlog::info("");
    spdlog::debug("");
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("");
    spdlog::critical("");
    spdlog::error("");
    spdlog::trace("");
    spdlog::warn("");

    spdlog::location::logger logger(spdlog::default_logger());
    logger.info("");
    logger.warn("");
    logger.error("");

    []
    {
        spdlog::info("__func__: {}", __func__);
        spdlog::info(std::source_location::current().function_name());
        spdlog::info("lambda");
    }();

    save_func("main");

    Object obj;
    obj.f();

    space::g();

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
