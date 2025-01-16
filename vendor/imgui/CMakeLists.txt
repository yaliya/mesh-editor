cmake_minimum_required(VERSION 3.29)
project(imgui)

set(CMAKE_CXX_STANDARD 20)

add_executable(imgui
        imgui.h
        imgui.cpp
        imconfig.h
        imgui_internal.h
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        backends/imgui_impl_glfw.h
        backends/imgui_impl_glfw.cpp)

target_link_libraries(imgui)

