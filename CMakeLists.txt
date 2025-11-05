cmake_minimum_required(VERSION 3.20)
project(COP3530_Hotel_Reviews_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SOURCES
        main.cpp
        Trie.cpp
        HeapSort.cpp

        # ImGui core
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp

        # ImGui backends
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)


include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/imgui
        ${CMAKE_SOURCE_DIR}/imgui/backends
        ${CMAKE_SOURCE_DIR}/glfw/include
)


if (WIN32)
    set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/glfw/lib-mingw-w64")
    link_directories(${GLFW_LIB_DIR})

    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} glfw3 opengl32 gdi32)
else()
    # For Linux/macOS users
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)
endif()


set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
