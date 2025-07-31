# cmake版本
cmake_minimum_required(VERSION 3.28...4.1)

# 項目名
project(windows_thread LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(boost_scope REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

add_executable(main)
target_sources(main
  PRIVATE FILE_SET CXX_MODULES FILES
    src/thread.ixx
    src/thread.detail.ixx
    src/mutex.ixx
    src/common.ixx
    src/event.ixx
    src/main.ixx
)
target_link_libraries(main
  PRIVATE
    fmt::fmt
    spdlog::spdlog
    Boost::scope
    Catch2::Catch2
)
target_compile_features(main PRIVATE cxx_std_23)
