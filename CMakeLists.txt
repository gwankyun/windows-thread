# cmake最低版本
cmake_minimum_required(VERSION 3.20)

# 啟用IDE目錄功能
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 包括模板
include(CMakePrintHelpers)

# 輸出目錄
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 項目名
project(windows-thread)

option(USE_LITE "use lite lib" ON)
cmake_print_variables(USE_LITE)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/W4;/MP;/Zc:__cplusplus;/std:c17;>")

find_package(spdlog)
cmake_print_variables(spdlog_FOUND)

# 生成執行文件
set(main main)
add_executable(${main})

target_sources(${main}
  PRIVATE
    src/main.cpp
    include/common.hpp
    include/mutex.hpp
    include/thread.hpp
)

target_link_libraries(${main}
  PRIVATE
    "$<$<TARGET_EXISTS:spdlog::spdlog_header_only>:spdlog::spdlog_header_only>"
)

target_compile_definitions(${main}
  PRIVATE
    "$<$<TARGET_EXISTS:spdlog::spdlog_header_only>:HAS_SPDLOG=1>"
    "$<$<BOOL:${USE_LITE}>:USE_LITE=1>"
)

target_include_directories(${main}
  PRIVATE
    include
)

# 設置C++標準
target_compile_features(${main} PRIVATE cxx_std_17)
