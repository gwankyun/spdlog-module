function(add_bundle_command)
  if(NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    return()
  endif()

  cmake_parse_arguments(ARG "" "TARGET;BUNDLE_DIRS;OBJDUMP" "" ${ARGN})

  add_custom_command(
    TARGET ${ARG_TARGET}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND}
      # gersemi: off
      -D apps="$<TARGET_FILE:${ARG_TARGET}>"
      -D dirs="${ARG_BUNDLE_DIRS}"
      "$<$<BOOL:${ARG_OBJDUMP}>:-D CMAKE_OBJDUMP=${ARG_OBJDUMP}>"
      # gersemi: on
      -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/fixup.cmake"
    COMMENT ""
  )
endfunction()
