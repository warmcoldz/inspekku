cmake_minimum_required(VERSION 3.22)

project(inspekku)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_library(inspekku_lib INTERFACE src/memory.h src/periodic_task.h)
target_include_directories(inspekku_lib INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(inspekku_lib INTERFACE ${Boost_LIBRARIES} Threads::Threads)

add_executable(inspekku src/main.cpp)
target_link_libraries(inspekku PRIVATE inspekku_lib)

add_executable(inspekku_tests src/memory_test.cpp)
target_link_libraries(inspekku_tests PRIVATE inspekku_lib GTest::GTest)
