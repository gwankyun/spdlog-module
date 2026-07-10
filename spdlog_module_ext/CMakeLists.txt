add_library(spdlog_module_ext)
target_sources(spdlog_module_ext
  PUBLIC
    FILE_SET CXX_MODULES
      FILES
        src/spdlog.util.ixx
        src/spdlog.ext.ixx
        src/spdlog.location.ixx
        src/spdlog.location.logger.ixx
        src/spdlog.location.alias.ixx
)
target_link_libraries(spdlog_module_ext
  PUBLIC
    spdlog_module
)
target_compile_features(spdlog_module_ext PUBLIC cxx_std_23)

cmake_print_variables(PROJECT_IS_TOP_LEVEL)

if(PROJECT_IS_TOP_LEVEL)
  include(GNUInstallDirs)

  install(
    TARGETS spdlog_module_ext
    EXPORT spdlog_module_extTargets
    FILE_SET CXX_MODULES
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spdlog_module_ext/modules
  )

  install(
    EXPORT spdlog_module_extTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/spdlog_module_ext
    NAMESPACE spdlog_module_ext::
  )

  include(CMakePackageConfigHelpers)

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/spdlog_module_extConfigVersion.cmake
    COMPATIBILITY ExactVersion
  )

  install(
    FILES
      cmake/spdlog_module_extConfig.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/spdlog_module_extConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/spdlog_module_ext
  )
endif()
