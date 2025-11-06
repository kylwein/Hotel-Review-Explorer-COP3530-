cmake_minimum_required(VERSION 3.20)
project(COP3530_Hotel_Reviews_Project)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${CMAKE_SOURCE_DIR}/imgui
        ${CMAKE_SOURCE_DIR}/imgui/backends
        ${CMAKE_SOURCE_DIR}/glfw/include
)

link_directories(${CMAKE_SOURCE_DIR}/glfw/lib-mingw-w64)


add_executable(COP3530_Hotel_Reviews_Project
        main.cpp
        Trie.h
        HeapSort.cpp
        HeapSort.h
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(COP3530_Hotel_Reviews_Project
        glfw3
        opengl32
)


