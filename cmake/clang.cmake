function(clang_generate_clangd target)
  if(
    NOT (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") #
    OR NOT (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
  )
    return()
  endif()

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
