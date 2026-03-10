# cmake最低版本
cmake_minimum_required(VERSION 4.3)

# 包含模塊
include(CMakePrintHelpers)

# 項目名
project(spdlog_module)

find_package(spdlog CONFIG REQUIRED)

add_executable(main)
target_sources(main
  PRIVATE
    FILE_SET CXX_MODULES
      FILES
        src/spdlog.ixx
  PRIVATE
    src/main.cpp
)
target_link_libraries(main
  PRIVATE
    spdlog::spdlog
)
target_compile_features(main PRIVATE cxx_std_23)
