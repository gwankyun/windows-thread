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

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/W4;/MP;/Zc:__cplusplus;/std:c17;>")

# 生成執行文件
set(main main)
add_executable(${main})

target_sources(${main}
  PRIVATE
    src/main.cpp
)

target_include_directories(${main} PRIVATE include)

target_compile_features(${main} PRIVATE cxx_std_17)
