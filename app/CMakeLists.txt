cmake_minimum_required(VERSION 3.28.0)
project(sketch1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# Fetch SFML, ImGui, and ImGui-SFML from GitHub
include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0)

# Disable SFML features not needed
option(SFML_BUILD_AUDIO "Build audio" OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)

# Fetch dependencies
FetchContent_MakeAvailable(SFML)
# find_package(SFML COMPONENTS Graphics Window System REQUIRED)


# Add the executable for the project
add_executable(sketch1 main.cpp sketch.cpp testsketch.cpp)

# Link SFML, ImGui, and ImGui-SFML
target_link_libraries(sketch1 PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
)
