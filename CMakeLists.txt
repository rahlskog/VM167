project(VM167)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_subdirectory(src)
include_directories(src)
add_subdirectory(test)
if (TestApp)
    add_subdirectory(TestApp)
endif(TestApp)
