function(add_bundle_command)
  if(NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    return()
  endif()

  cmake_parse_arguments(ARG "" "TARGET;BUNDLE_DIRS" "" ${ARGN})

  if(NOT ARG_BUNDLE_DIRS)
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH ARG_BUNDLE_DIRS)
  endif()

  add_custom_command(
    TARGET ${ARG_TARGET}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND}
      -D apps="$<TARGET_FILE:${ARG_TARGET}>"
      -D dirs="${ARG_BUNDLE_DIRS}"
      -D CMAKE_OBJDUMP="${CMAKE_OBJDUMP}"
      -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/fixup.cmake"
    COMMENT ""
  )
endfunction()
