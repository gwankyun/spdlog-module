#include <spdlog_module/spdlog.h>

import std;
import spdlog;

int main(int _argc, char* _argv[])
{
    spdlog::set_pattern("[%C-%m-%d %T.%e] [%^%L%$] [%-8!!:%4#] %v");

    SPDLOG_INFO("");

    return 0;
}
