# cmake最低版本
cmake_minimum_required(VERSION 4.3)

# 包含模塊
include(CMakePrintHelpers)

include(cmake/cxx_module.cmake)
include(cmake/bundle.cmake)

# CMake對Clang的import std;支持還在試驗階段
get_import_std(import_std_uuid)
cmake_print_variables(import_std_uuid)

set(
  CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
  ${import_std_uuid}
  CACHE STRING
  "UUID for import std"
)

cmake_print_variables(USE_IMPORT_STD_UUID)

project(spdlog_module
  VERSION 1.0.0
)

# 全局启用標準庫模塊
enable_module_std(ON)

cmake_print_variables(USE_IMPORT_STD)

find_package(spdlog CONFIG REQUIRED)
# find_package(Catch2 CONFIG REQUIRED)

cmake_print_variables(PROJECT_IS_TOP_LEVEL)

add_subdirectory(spdlog_module)
# add_subdirectory(spdlog_module_ext)

#[[
add_executable(main)
target_sources(main
  PRIVATE
    src/main.cpp
)
target_link_libraries(main
  PRIVATE
    spdlog_module_ext
    Catch2::Catch2
)
target_compile_features(main PRIVATE cxx_std_23)
#]]

add_executable(use_macro)
target_sources(use_macro
  PRIVATE
    src/use_macro.cpp
)
target_link_libraries(use_macro
  PRIVATE
    spdlog_module
    # Catch2::Catch2
)
target_compile_features(use_macro PRIVATE cxx_std_23)

if(USE_IMPORT_STD STREQUAL "ON")
  target_compile_definitions(use_macro PRIVATE USE_IMPORT_STD=1)
endif()

add_bundle_command(
  TARGET use_macro
  BUNDLE_DIRS "${dirs}"
  OBJDUMP "${CMAKE_OBJDUMP}"
)

include(cmake/clang.cmake) # clang_generate_clangd

clang_generate_clangd(generate_clangd)
