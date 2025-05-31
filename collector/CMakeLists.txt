find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_library(inspekku_collector_lib INTERFACE src/memory.h src/periodic_task.h)
target_include_directories(inspekku_collector_lib INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(inspekku_collector_lib INTERFACE ${Boost_LIBRARIES} Threads::Threads)

add_executable(inspekku_collector src/main.cpp)
target_link_libraries(inspekku_collector PRIVATE inspekku_collector_lib)

add_executable(inspekku_collector_tests src/memory_test.cpp)
target_link_libraries(inspekku_collector_tests PRIVATE inspekku_collector_lib GTest::GTest)
