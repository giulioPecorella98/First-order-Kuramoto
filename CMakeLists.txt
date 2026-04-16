cmake_minimum_required(VERSION 3.10)

project(First-order-Kuramoto-1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

include_directories(simulation_engine/include)

set(SOURCES
    simulation_engine/src/main.cpp
    simulation_engine/src/io/save.cpp
    simulation_engine/src/numerical_scheme/finite_difference.cpp
    simulation_engine/src/simulation_parameters/initial_conditions.cpp
    simulation_engine/src/simulation_parameters/parameters.cpp
    simulation_engine/src/numerical_scheme/order_parameter.cpp
)

add_executable(kuramoto ${SOURCES})

set_target_properties(kuramoto PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/simulation_engine
)

target_compile_definitions(kuramoto PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(kuramoto m)
