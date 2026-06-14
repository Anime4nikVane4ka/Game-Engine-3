cmake_minimum_required(VERSION 3.31)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(ecs-demo
    VERSION 1.0
    LANGUAGES CXX)

add_subdirectory(dependencies)
add_subdirectory(src)
