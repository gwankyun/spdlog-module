# 為目標啟用標準庫模塊
function(target_enable_module_std tgt enable_std)
  # 檢查當前C++編譯器是否為Clang或GNU
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set_target_properties(${tgt} PROPERTIES CXX_MODULE_STD ${enable_std})
  endif()
endfunction()

# 全局啟用標準庫模塊
function(enable_module_std enable_std)
  # 檢查當前C++編譯器是否為Clang或GNU
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" AND NOT CMAKE_GENERATOR MATCHES "Visual Studio")
    set(CMAKE_CXX_MODULE_STD ${enable_std} PARENT_SCOPE)
  endif()
endfunction()

# 獲取當前C++標準庫模塊的UUID
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
    set(uuid "${import_std_440}") # [4.4.0-rc3, 4.4.0]有效
  elseif(${ver} VERSION_GREATER_EQUAL ${v430_rc1} AND ${ver} VERSION_LESS ${v440_rc3})
    set(uuid "${import_std_430}") # [4.3.0-rc1, 4.4.0-rc3)有效
  elseif(${ver} VERSION_GREATER_EQUAL ${v410_rc1} AND ${ver} VERSION_LESS ${v430_rc1})
    set(uuid "${import_std_410}") # [4.1.0-rc1, 4.3.0-rc1)有效
  elseif(${ver} VERSION_GREATER_EQUAL ${v400_rc1} AND ${ver} VERSION_LESS ${v410_rc1})
    set(uuid "${import_std_400}") # [4.0.0-rc1, 4.1.0-rc1)有效
  elseif(${ver} VERSION_GREATER_EQUAL ${v330_rc1} AND ${ver} VERSION_LESS ${v400_rc1})
    set(uuid "${import_std_330}") # [3.30-rc1, 4.0.0-rc1)有效
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
