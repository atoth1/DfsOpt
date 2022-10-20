cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(DfsOpt VERSION 0.0.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXMLXX REQUIRED IMPORTED_TARGET libxml++-2.6)
add_library(LIBXMLXX::LIBXMLXX ALIAS PkgConfig::LIBXMLXX)

add_subdirectory(src)

include(CTest)
include(GoogleTest)
find_package(GTest)
option(ENABLE_TESTING "Enable building of unit tests." ON)

if (GTest_FOUND AND ENABLE_TESTING)
  add_subdirectory(test)
endif()