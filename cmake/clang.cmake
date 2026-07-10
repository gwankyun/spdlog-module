function(clang_generate_clangd target)
  # gersemi: off
  add_custom_target(
    ${target}
    COMMAND
      ${CMAKE_COMMAND}
      -D SOURCE_DIR="${CMAKE_SOURCE_DIR}"
      -D BINARY_DIR="${CMAKE_BINARY_DIR}"
      -D CONFIG="$<CONFIG>"
      -P "${CMAKE_SOURCE_DIR}/cmake/generate_clangd.cmake"
    COMMENT "生成.clangd配置文件"
  )
  # gersemi: on
endfunction()
