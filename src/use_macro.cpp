#define SPDLOG_MACRO_ONLY
#include <spdlog/spdlog.h>

#ifdef USE_IMPORT_STD
import std;
#endif

import spdlog;

int main(int _argc, char* _argv[])
{
    spdlog::set_pattern("[%C-%m-%d %T.%e] [%^%L%$] [%-8!!:%4#] %v");

    SPDLOG_INFO("");

    return 0;
}
