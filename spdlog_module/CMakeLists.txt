add_library(spdlog_module)
target_sources(spdlog_module
  PUBLIC
    FILE_SET CXX_MODULES
      FILES
        src/spdlog.common.ixx
        src/spdlog.util.ixx
        src/spdlog.ixx
        src/spdlog.location.ixx
        src/spdlog.location.logger.ixx
        src/spdlog.location.alias.ixx
)
target_link_libraries(spdlog_module
  PUBLIC
    spdlog::spdlog
)
target_compile_features(spdlog_module PUBLIC cxx_std_23)
add_library(spdlog::module ALIAS spdlog_module)
