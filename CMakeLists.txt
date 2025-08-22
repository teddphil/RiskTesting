cmake_minimum_required(VERSION 3.14)
project(atm_risk_testing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(atm
    src/Metrics.cpp
    src/Reporter.cpp
    src/Runner.cpp
    src/Utils.cpp
)
target_include_directories(atm PUBLIC include)

add_executable(atm_runner
    tests/test_volatility.cpp
    models/BasicVaR.cpp
)
target_link_libraries(atm_runner PRIVATE atm)
