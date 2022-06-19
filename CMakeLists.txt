cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ")
project(MultiSubgraphMatching CXX)


#set(GTEST_INCLUDE src/include/thirdpart)
#add_subdirectory(./src/include/thirdpart/googletest)
include_directories(src)
include_directories(src/include/thirdpart/easylog)



file(GLOB_RECURSE ALL_SRCS "*.h" "*.cpp" "*.cc")
add_library(SRCS STATIC ${ALL_SRCS}  )

option(LOG "options for log (debug)" OFF)

if(LOG)
    add_definitions(-DLOGGING)
endif(LOG)

AUX_SOURCE_DIRECTORY(./src DIR_SRCS)
add_executable(Matching ${DIR_SRCS} )
target_link_libraries(Matching SRCS)


#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")