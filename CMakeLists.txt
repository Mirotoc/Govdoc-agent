cmake_minimum_required(VERSION 3.16)
project(govdoc CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options(/utf-8 /W4)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

add_library(govcore src/document.cpp src/text.cpp src/rules.cpp src/llm.cpp src/label.cpp)
if(WIN32)
  target_link_libraries(govcore ws2_32)
endif()

add_executable(govdoc src/main.cpp)
target_link_libraries(govdoc govcore)

enable_testing()
add_executable(rules_test tests/rules_test.cpp)
target_link_libraries(rules_test govcore)
add_test(NAME rules_test COMMAND rules_test)
