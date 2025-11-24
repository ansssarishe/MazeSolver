cmake_minimum_required(VERSION 3.20)
project(Maze)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH "opt/homebrew")

find_package(raylib REQUIRED)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui")
set(RLIMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/rlImGui")

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${RLIMGUI_DIR}/rlImGui.cpp
)

add_executable(app
        src/main.cpp
        ${IMGUI_SOURCES}
)

target_link_libraries(app PRIVATE raylib)

target_include_directories(app PRIVATE
        src
        ${IMGUI_DIR}    # <--- Points to libs/imgui
        ${RLIMGUI_DIR}  # <--- Points to libs/rlImGui
)