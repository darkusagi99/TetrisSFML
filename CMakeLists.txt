cmake_minimum_required(VERSION 3.2)
project(TetrisSFML)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(TetrisSFML ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(TetrisSFML sfml-system sfml-window sfml-graphics sfml-audio sfml-network)