cmake_minimum_required(VERSION 3.20)

project(translator)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)


find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(translator src/main.cpp )

target_link_libraries(translator PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
