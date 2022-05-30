cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ")
project(MultiSubgraphMatching CXX)


set(GTEST_INCLUDE src/include/thirdpart)
add_subdirectory(./src/include/thirdpart/googletest)
include_directories(src)



file(GLOB_RECURSE ALL_SRCS "*.h" "*.cpp")
add_library(SRCS STATIC ${ALL_SRCS}  )
include_directories(${ALL_SRCS})


add_executable(Matching src/main.cpp )
target_link_libraries(Matching SRCS)