cmake_minimum_required(VERSION 3.16)

project(CarServiceCenter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(car_service_core
    src/ServiceRequest.cpp
)

target_include_directories(car_service_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(car_service_core PRIVATE /W4)
else()
    target_compile_options(car_service_core PRIVATE -Wall -Wextra -pedantic)
endif()

enable_testing()

add_executable(service_request_tests
    tests/ServiceRequestTests.cpp
)

target_link_libraries(service_request_tests PRIVATE car_service_core)

add_test(NAME service_request_tests COMMAND service_request_tests)
