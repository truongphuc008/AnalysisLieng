cmake_minimum_required(VERSION 3.10)
project(LiengSimulation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tắt cảnh báo MSVC về localtime (nếu dùng MSVC)
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Thu thập tất cả file nguồn
set(SOURCES
    main.cpp
    Card.cpp
    Deck.cpp
    Hand.cpp
    HandEvaluator.cpp
    Player.cpp
    GameRound.cpp
    Simulation.cpp
    Visualizer.cpp
)

add_executable(LiengSimulation ${SOURCES})

# Bật tối ưu hóa Release để chạy 100k ván nhanh hơn
target_compile_options(LiengSimulation PRIVATE
    $<$<CONFIG:Release>:-O2>
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
