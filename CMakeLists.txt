cmake_minimum_required(VERSION 3.28.0)
project(imgui-test VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
# set(IMGUI_SFML_FIND_SFML OFF)  # Ignore system SFML

# Fetch SFML, ImGui, and ImGui-SFML from GitHub
include(FetchContent)
include_directories(${CMAKE_SOURCE_DIR}/external)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8
)

FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v3.0
)

# Disable SFML features not needed
option(SFML_BUILD_AUDIO "Build audio" OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)

# Fetch dependencies
FetchContent_MakeAvailable(SFML)
# find_package(SFML COMPONENTS Graphics Window System REQUIRED)
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
FetchContent_MakeAvailable(imgui-sfml)


# Add the executable for the project
add_executable(imgui-test main.cpp external/TextEditor.cpp)

# Include paths for ImGui and ImGui-SFML
target_include_directories(imgui-test PRIVATE 
    ${imgui_SOURCE_DIR} 
    ${imgui-sfml_SOURCE_DIR}
)

# Link SFML, ImGui, and ImGui-SFML
target_link_libraries(imgui-test PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
    ImGui-SFML::ImGui-SFML
)
