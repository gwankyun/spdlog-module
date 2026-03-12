# cmake最低版本
cmake_minimum_required(VERSION 4.3)

# 包含模塊
include(CMakePrintHelpers)

# 項目名
project(spdlog_module)

find_package(spdlog CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

add_subdirectory(spdlog_module)

add_executable(main)
target_sources(main
  PRIVATE
    src/main.cpp
)
target_link_libraries(main
  PRIVATE
    spdlog::module
    Catch2::Catch2
)
target_compile_features(main PRIVATE cxx_std_23)
