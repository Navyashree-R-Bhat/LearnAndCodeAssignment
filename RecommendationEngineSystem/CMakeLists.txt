cmake_minimum_required(VERSION 3.16.3)

project(RecommendationEngineSystem Version1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(RecommendationEngineSystem src/main.cpp src/*.cpp)

target_include_directories(RecommendationEngineSystem PRIVATE inc)