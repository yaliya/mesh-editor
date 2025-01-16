cmake_minimum_required(VERSION 3.29)
project(level_editor)

set(CMAKE_CXX_STANDARD 20)

# Find and link GLFW
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(/usr/local/include)
link_directories(/usr/local/lib vendor/nativefiledialog/build/lib/Release/x64)

add_executable(level_editor main.cpp
        src/types/vertex.h
        src/primitives/cube.h
        src/primitives/cube.cpp
        vendor/glad.c
        src/util/shader.h
        src/util/shader.cpp
        src/primitives/torus.h
        src/primitives/torus.cpp
        src/managers/scene_manager.h
        src/managers/scene_manager.cpp
        src/camera.h
        src/camera.cpp
        vendor/imgui/imgui.h
        vendor/imgui/imgui.cpp
        vendor/imgui/imconfig.h
        vendor/imgui/imgui_styles.h
        vendor/imgui/imgui_internal.h
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imstb_rectpack.h
        vendor/imgui/imstb_textedit.h
        vendor/imgui/imstb_truetype.h
        vendor/imgui/backends/imgui_impl_glfw.h
        vendor/imgui/backends/imgui_impl_glfw.cpp
        vendor/imgui/backends/imgui_impl_opengl3.h
        vendor/imgui/backends/imgui_impl_opengl3.cpp
        vendor/imgui/imgui_styles.h
        vendor/imguizmo/ImGuizmo.h
        vendor/imguizmo/ImGuizmo.cpp
        src/managers/asset_manager.h
        src/assets/texture.h
        src/assets/texture.cpp
        vendor/stbi_image.h
        src/types/mesh.h
        src/types/mesh.cpp
        src/systems/input_system.h
        src/systems/base_system.h
        src/systems/input_system.cpp
        src/systems/render_system.h
        src/systems/render_system.cpp
        src/primitives/cone.h
        src/primitives/cone.cpp
        src/primitives/sphere.h
        src/primitives/sphere.cpp
        src/primitives/plane.h
        src/primitives/plane.cpp
        src/window.h
        src/window.cpp
        src/types/model.h
        src/types/model.cpp
        src/assets/asset.h
        src/primitives/grid.h
        src/primitives/grid.cpp
        src/core/command.h
        src/core/command/select_vertex_command.h
        src/core/command/select_vertex_command.cpp
        src/managers/command_manager.h
        src/core/command/select_edge_command.h
        src/core/command/select_edge_command.cpp
        src/core/command/select_face_command.h
        src/core/command/select_face_command.cpp
        src/managers/command_manager.cpp
        src/core/command/translate_vertex_command.h
        src/managers/event_manager.h
        src/core/command/translate_vertex_command.cpp
        src/core/command/translate_face_command.h
        src/core/command/translate_face_command.cpp
        src/assets/object3d.h
        src/assets/object3d.cpp
        src/util/geom.h
        src/core/state/vertex_selection_state.h
        src/core/state/gizmo_matrix_state.h
        src/core/state/mesh_selection_state.h
        src/core/listener/reposition_gizmo_listener.h
        src/core/event/reposition_gizmo_event.h
        src/core/event.h
        src/core/state/face_selection_state.h
        src/core/state/edge_selection_state.h
        src/systems/gui_system.h
        src/systems/gui_system.cpp
        src/core/command/create_scene_object_command.h
        src/core/command/create_scene_object_command.cpp
        src/util/singleton.h
        src/core/event/scene_rendered_event.h
        src/core/listener/scene_rendered_listener.h
        src/core/math/transform.h
        src/core/command/translate_mesh_command.h
        src/core/command/translate_mesh_command.cpp
)

target_link_libraries(level_editor glfw assimp nfd ${OPENGL_gl_LIBRARY})

