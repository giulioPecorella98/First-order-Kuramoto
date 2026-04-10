cmake_minimum_required(VERSION 3.10)

project(First-order-Kuramoto-1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

include_directories(include)

set(SOURCES
    src/main.cpp
    src/io/save.cpp
    src/numerical_scheme/finite_difference.cpp
    src/simulation_parameters/initial_conditions.cpp
    src/simulation_parameters/parameters.cpp
    src/plot/plot.cpp
)

add_executable(kuramoto.exe ${SOURCES})

# link math library (-lm)
target_link_libraries(kuramoto.exe m)
