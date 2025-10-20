cmake_minimum_required(VERSION 3.15)
project(cards_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/gameStructure.cpp
    src/playerSets.cpp
    src/holdEmRules.cpp
)

# Include directory
include_directories(src/includes)

# Find SDL2 (Homebrew)
find_package(SDL2 REQUIRED)

if (NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found! Try setting CMAKE_PREFIX_PATH=/opt/homebrew/opt/sdl2")
endif()

include_directories(${SDL2_INCLUDE_DIRS})

# Build executable
add_executable(cards_engine ${SOURCES})
target_link_libraries(cards_engine ${SDL2_LIBRARIES})
