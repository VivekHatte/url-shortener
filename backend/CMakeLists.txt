cmake_minimum_required(VERSION 3.16)
project(url_shortener CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Drogon CONFIG REQUIRED)

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
     src/controllers/*.cpp src/utils/*.cpp)

add_library(url_shortener_obj OBJECT ${LIB_SOURCES})
target_include_directories(url_shortener_obj PUBLIC src)
target_link_libraries(url_shortener_obj PUBLIC Drogon::Drogon)

add_executable(url_shortener src/main.cpp)
target_link_libraries(url_shortener PRIVATE url_shortener_obj)

option(BUILD_TESTS "Build the GoogleTest unit & integration tests" OFF)
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
