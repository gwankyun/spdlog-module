include(CMakePrintHelpers)

cmake_print_variables(SOURCE_DIR)
cmake_print_variables(BINARY_DIR)
cmake_print_variables(CONFIG)

# 讀預設.clangd文件
file(READ "${SOURCE_DIR}/cmake/.clangd" clang_d)

cmake_print_variables(clang_d)

# 查找${CONFIG}配置下的所有.pcm文件
file(GLOB_RECURSE pcm_file_list "${BINARY_DIR}/**/*.pcm")

if(pcm_file_list)
  foreach(pcm_file IN LISTS pcm_file_list)
    string(REGEX MATCH "${CONFIG}/([^/]+)\.pcm$" match_result ${pcm_file})
    if(match_result)
      message(STATUS "found ${pcm_file}")
      string(
        APPEND clang_d
        "    - -fmodule-file=${CMAKE_MATCH_1}=${pcm_file}\n"
      )
    endif()
  endforeach()
endif()

set(clang_config "${SOURCE_DIR}/.clangd")

# 寫入.clangd配置文件
file(WRITE "${clang_config}" "${clang_d}")
