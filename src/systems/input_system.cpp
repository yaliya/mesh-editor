#include "input_system.h"

#include "../window.h"
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imguizmo/ImGuizmo.h"
#include "../core/command/select_edge_command.h"
#include "../core/command/select_face_command.h"
#include "../core/command/select_vertex_command.h"
#include "../core/command/translate_face_command.h"
#include "../core/command/translate_mesh_command.h"
#include "../core/command/translate_vertex_command.h"

void InputSystem::update(const double &deltaTime) {
    const auto scene = window->getScene();
    const auto camera = scene->getCamera();
    const auto commands = window->getCommandManager();

    if (ImGui::IsMouseDragging(1)) {
        ImVec2 mousePosition = ImGui::GetMousePos();
        camera->trackMouse(mousePosition.x, mousePosition.y);
    }

    if (ImGui::IsMouseReleased(1)) {
        camera->resetMouse();
    }

    if (ImGui::IsKeyDown(ImGuiKey_W)) {
        camera->moveForward();
    }

    if (ImGui::IsKeyDown(ImGuiKey_A)) {
        camera->moveLeft();
    }

    if (ImGui::IsKeyDown(ImGuiKey_S)) {
        camera->moveBackwards();
    }

    if (ImGui::IsKeyDown(ImGuiKey_D)) {
        camera->moveRight();
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        const ImVec2 mousePosition = ImGui::GetMousePos();
        camera->screenToWorld(mousePosition.x, mousePosition.y);

        if (!ImGuizmo::IsOver()) {
            if (scene->getSelectionMode() == SelectionMode::VERTICES) {
                commands->execute(std::make_unique<SelectVertexCommand>(scene), ImGui::IsKeyDown(ImGuiKey_LeftCtrl));
            }

            if (scene->getSelectionMode() == SelectionMode::EDGES) {
                commands->execute(std::make_unique<SelectEdgeCommand>(scene), ImGui::IsKeyDown(ImGuiKey_LeftCtrl));
            }

            if (scene->getSelectionMode() == SelectionMode::FACES) {
                commands->execute(std::make_unique<SelectFaceCommand>(scene), ImGui::IsKeyDown(ImGuiKey_LeftCtrl));
            }
        }
    }

    if (ImGuizmo::IsUsing()) {
        if (scene->getSelectionMode() == SelectionMode::OBJECTS) {
            commands->loop(std::make_unique<TranslateMeshCommand>(scene), ImGui::IsMouseDragging(ImGuiMouseButton_Left));
        }
        if (scene->getSelectionMode() == SelectionMode::VERTICES) {
            commands->loop(std::make_unique<TranslateVertexCommand>(scene), ImGui::IsMouseDragging(ImGuiMouseButton_Left));
        }
        if (scene->getSelectionMode() == SelectionMode::FACES) {
            commands->loop(std::make_unique<TranslateFaceCommand>(scene), ImGui::IsMouseDragging(ImGuiMouseButton_Left));
        }
    }

    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
        if (ImGui::IsKeyPressed(ImGuiKey_Z)) {
            commands->undo();
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Y)) {
            commands->redo();
        }
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        commands->done();
    }
}


