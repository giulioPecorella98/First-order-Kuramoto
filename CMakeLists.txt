cmake_minimum_required(VERSION 3.10)

project(First-order-Kuramoto-1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

include_directories(simulation_one/include)

set(SOURCES_one
    simulation_one/src/simulation_one_engine.cpp
    simulation_one/src/numerical_scheme/finite_difference.cpp
    simulation_one/src/simulation_parameters/initial_conditions.cpp
    simulation_one/src/simulation_parameters/parameters.cpp
    simulation_one/src/numerical_scheme/order_parameter.cpp
)


add_executable(simulation_one ${SOURCES_one})

set_target_properties(simulation_one PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/simulation_one
)

target_compile_definitions(simulation_one PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(simulation_one m)

set(SOURCES_two
    simulation_one/src/simulation_two_engine.cpp
    simulation_one/src/numerical_scheme/finite_difference.cpp
    simulation_one/src/simulation_parameters/initial_conditions.cpp
    simulation_one/src/simulation_parameters/parameters_two.cpp
    simulation_one/src/numerical_scheme/order_parameter.cpp
)


add_executable(simulation_two ${SOURCES_two})

set_target_properties(simulation_two PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/simulation_two
)

target_compile_definitions(simulation_two PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(simulation_two m)

