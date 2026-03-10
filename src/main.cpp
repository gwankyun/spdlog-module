//#ifndef WIN32_LEAN_AND_MEAN
//#  define WIN32_LEAN_AND_MEAN
//#endif

//#include <boost/process.hpp>
//
//#include <boost/asio/read.hpp>
//#include <boost/asio/readable_pipe.hpp>
//#include <boost/system/error_code.hpp>

import std;
import spdlog;

//namespace proc = boost::process;
//namespace asio = boost::asio;

//int process_example()
//{
//    asio::io_context ctx;
//    const auto exe = proc::environment::find_executable("git");
//    proc::popen c{ctx, exe, {"--version"}};
//
//    std::string line;
//    boost::system::error_code ec;
//
//    auto sz = asio::read(c, asio::dynamic_buffer(line), ec);
//    //SPDLOG_INFO("sz: {}", sz);
//    //SPDLOG_INFO("ec: {}", ec.message());
//
//    //SPDLOG_INFO("git version: {}", line);
//
//    c.wait();
//    return c.exit_code();
//}

int main(int _argc, char* _argv[])
{
    spdlog::set_pattern("[%C-%m-%d %T.%e] [%^%L%$] [%-8!!:%4#] %v");

    //spdlog::get().info("yes");
    //spdlog::get().debug("");
    //spdlog::set_level(spdlog::level::debug);
    //spdlog::get().debug("");
    //spdlog::warn("");
    //std::print(std::source_location::current().function_name());

    spdlog::info("");
    spdlog::debug("");
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("");
    spdlog::critical("");
    spdlog::error("");
    spdlog::trace("");
    spdlog::warn("");
    []
    {
        std::cout << std::source_location::current().function_name()
                  << std::endl;
        spdlog::info("lambda");
    }();
    return 0;
}
