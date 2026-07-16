# 為目標啟用標準庫模塊
function(target_enable_module_std tgt enable_std)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set_target_properties(${tgt} PROPERTIES CXX_MODULE_STD ${enable_std})
  endif()
endfunction()

# 全局啟用標準庫模塊
function(enable_module_std enable_std)
  if(CMAKE_CXX_COMPILER MATCHES "clang-cl" OR CMAKE_GENERATOR MATCHES "Visual Studio")
    set(enable_std OFF)
  endif()

  get_import_std(import_std_uuid)
  cmake_print_variables(import_std_uuid)

  set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD ${import_std_uuid} PARENT_SCOPE)
  set(CMAKE_CXX_MODULE_STD ${enable_std} PARENT_SCOPE)
endfunction()

function(precompile_std_gcm result_var)
  if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(${result_var} TRUE PARENT_SCOPE)
    return()
  endif()

  if(CMAKE_VERSION VERSION_GREATER_EQUAL 4.2)
    execute_process(
      COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libstdc++.modules.json
      OUTPUT_VARIABLE modules_json_path
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(EXISTS "${modules_json_path}")
      set(CMAKE_CXX_STDLIB_MODULES_JSON "${modules_json_path}" PARENT_SCOPE)
      message(STATUS "Using GCC modules.json: ${modules_json_path}")
      set(${result_var} TRUE PARENT_SCOPE)
    else()
      message(STATUS "GCC modules.json not found, CMake will attempt auto-discovery")
      set(${result_var} TRUE PARENT_SCOPE)
    endif()
    return()
  endif()

  # GCC推薦使用-print-file-name定位內部文件，但MinGW下返回相對路徑
  execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=bits/std.cc
    OUTPUT_VARIABLE std_cc_path
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # MinGW回退：從CMake隱式包含目錄中搜索std.cc
  if(NOT EXISTS "${std_cc_path}")
    set(std_cc_path "")
    foreach(dir ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
      string(REGEX MATCH "c\\+\\+/[0-9\\.]+[0-9]" cxx_ver_match "${dir}")
      string(REGEX REPLACE ".*c\\+\\+/" "" cxx_ver "${cxx_ver_match}")
      if(cxx_ver)
        set(candidate "${dir}/c++/${cxx_ver}/bits/std.cc")
        if(EXISTS "${candidate}")
          set(std_cc_path "${candidate}")
          break()
        endif()
      endif()
    endforeach()
  endif()

  if(NOT std_cc_path)
    message(STATUS "GCC std module source not found, skipping precompile")
    set(${result_var} FALSE PARENT_SCOPE)
    return()
  endif()

  # 若std.gcm已存在則跳過（避免重複預編譯）
  set(std_gcm "${CMAKE_BINARY_DIR}/gcm.cache/std.gcm")
  if(EXISTS "${std_gcm}")
    set(${result_var} TRUE PARENT_SCOPE)
    return()
  endif()

  # 預編譯std.cc爲std.gcm，GCC編譯時會在gcm.cache/下查找模塊緩存
  message(STATUS "Precompiling GCC std module: ${std_cc_path} -> ${std_gcm}")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/gcm.cache")
  execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -std=c++23 -fmodules -c "${std_cc_path}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    RESULT_VARIABLE result
  )
  if(result EQUAL 0)
    message(STATUS "GCC std module precompiled successfully")
    set(${result_var} TRUE PARENT_SCOPE)
  else()
    message(WARNING "GCC std module precompile failed (result=${result})")
    set(${result_var} FALSE PARENT_SCOPE)
  endif()
endfunction()

function(get_import_std result)
  set(uuid "")
  set(import_std_330 "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
  set(import_std_400 "a9e1cf81-9932-4810-974b-6eccaf14e457")
  set(import_std_410 "d0edc3af-4c50-42ea-a356-e2862fe7a444")
  set(import_std_430 "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
  set(import_std_440 "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
  set(ver ${CMAKE_VERSION})

  set(v330_rc1 "3.30.0-rc1")
  set(v400_rc1 "4.0.0-rc1")
  set(v410_rc1 "4.1.0-rc1")
  set(v430_rc1 "4.3.0-rc1")
  set(v440_rc3 "4.4.0-rc3")
  set(v440 "4.4.0")

  if(${ver} VERSION_GREATER ${v440})
    message(WARNING "CMAKE_VERSION ${ver} not tested")
    set(uuid "${import_std_440}")
  elseif(${ver} VERSION_EQUAL ${v440_rc3} AND ${ver} VERSION_LESS_EQUAL ${v440})
    set(uuid "${import_std_440}")
  elseif(${ver} VERSION_GREATER_EQUAL ${v430_rc1} AND ${ver} VERSION_LESS ${v440_rc3})
    set(uuid "${import_std_430}")
  elseif(${ver} VERSION_GREATER_EQUAL ${v410_rc1} AND ${ver} VERSION_LESS ${v430_rc1})
    set(uuid "${import_std_410}")
  elseif(${ver} VERSION_GREATER_EQUAL ${v400_rc1} AND ${ver} VERSION_LESS ${v410_rc1})
    set(uuid "${import_std_400}")
  elseif(${ver} VERSION_GREATER_EQUAL ${v330_rc1} AND ${ver} VERSION_LESS ${v400_rc1})
    set(uuid "${import_std_330}")
  else()
    message(FATAL_ERROR "CMAKE_VERSION ${ver} not supported")
  endif()

  set(${result} "${uuid}" PARENT_SCOPE)
endfunction()

function(check_module_support result_var)
  list(
    APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
    CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    CMAKE_CXX_MODULE_STD
    CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS
  )

  try_compile(
    ${result_var}
    SOURCES_TYPE CXX_MODULE
    SOURCE_FROM_CONTENT
      test_module.ixx
      [=[
      export module test_module;
      export int test_add(int a, int b)
      {
          return a + b;
      }
      ]=]
    SOURCES_TYPE NORMAL
    SOURCE_FROM_CONTENT
      main.cpp
      [=[
      import test_module;
      int main()
      {
          return test_add(1, 2) - 3;
      }
      ]=]
    CXX_STANDARD 23
  )
  set(${result_var} ${${result_var}} PARENT_SCOPE)
endfunction()

function(check_import_std_support result_var)
  list(
    APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
    CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    CMAKE_CXX_MODULE_STD
    CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS
  )

  try_compile(
    ${result_var}
    SOURCE_FROM_CONTENT
      main.cpp
      [=[
      import std;
      int main()
      {
          std::cout << 0 << std::endl;
          return 0;
      }
      ]=]
    CXX_STANDARD 23
  )
  set(${result_var} ${${result_var}} PARENT_SCOPE)
endfunction()

function(check_export_import_header_support result_var)
  list(
    APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
    CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    CMAKE_CXX_MODULE_STD
    CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS
  )

  try_compile(
    ${result_var}
    PROJECT check_export_import_header
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/cmake/checks/export-import-header"
    BINARY_DIR "${CMAKE_BINARY_DIR}/CMakeFiles/TryCompile-export-import-header"
  )
  set(${result_var} ${${result_var}} PARENT_SCOPE)
endfunction()
