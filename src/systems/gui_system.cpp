#include "gui_system.h"

#include <glm/gtc/type_ptr.hpp>

#include "../window.h"
#include "render_system.h"

#include "../assets/object3d.h"
#include "../primitives/cone.h"
#include "../primitives/cube.h"
#include "../primitives/plane.h"
#include "../primitives/sphere.h"
#include "../primitives/torus.h"

#include "../managers/asset_manager.h"
#include "../managers/event_manager.h"
#include "../core/state/gizmo_matrix_state.h"
#include "../core/event/reposition_gizmo_event.h"
#include "../core/listener/reposition_gizmo_listener.h"

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/font_awesome5.h"
#include "../../vendor/imguizmo/ImGuizmo.h"
#include "../../vendor/imgui/imgui_styles.h"
#include "../../vendor/imgui/imgui_internal.h"
#include "../../vendor/imgui/backends/imgui_impl_glfw.h"
#include "../../vendor/imgui/backends/imgui_impl_opengl3.h"
#include "../../vendor/nativefiledialog/src//include/nfd.h"
#include "../core/command/create_scene_object_command.h"

GUISystem::GUISystem(const std::shared_ptr<Window> &window) : BaseSystem(window) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    float iconFontSize = 24 * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;

    io.Fonts->AddFontFromFileTTF("data/fonts/Roboto-Medium.ttf", 16.0f);
    io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges );

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::SetGUIStyle(ImGui::FUTURE_DARK);

    ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true);
    ImGui_ImplOpenGL3_Init();
}


void GUISystem::update(const double &deltaTime) {
    const auto scene = window->getScene();
    const auto commandManager = window->getCommandManager();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));

    if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) {
                    scene->clear();
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                }
                if (ImGui::MenuItem("Save as..")) {
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo")) {
                    window->getCommandManager()->undo();
                }
                if (ImGui::MenuItem("Redo")) {
                    window->getCommandManager()->redo();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Create")) {
                std::shared_ptr<Model> model = nullptr;

                if (ImGui::MenuItem("Cube")) {
                    model = std::make_shared<Cube>(1, 2);
                }
                if (ImGui::MenuItem("Torus")) {
                    model = std::make_shared<Torus>(64, 32);
                }
                if (ImGui::MenuItem("Cone")) {
                    model = std::make_shared<Cone>(1, 2, 32, 8);
                }
                if (ImGui::MenuItem("Sphere")) {
                    model = std::make_shared<Sphere>(32, 32, 1);
                }
                if (ImGui::MenuItem("Plane")) {
                    model = std::make_shared<Plane>(12, 12, 6, 6);
                }
                if (ImGui::MenuItem("Import from file")) {
                    nfdchar_t* importFile = nullptr;

                    if (NFD_OpenDialog(nullptr, nullptr, &importFile) == NFD_OKAY) {
                        if (const auto asset = AssetManager::load<Object3D>(importFile)) {
                            model = std::make_shared<Model>(asset->getMeshes());
                        }
                    }
                }

                commandManager->execute(std::make_unique<CreateSceneObjectCommand>(scene, model), false);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Selection")) {
                if (ImGui::MenuItem("Object Mode")) {
                }
                if (ImGui::MenuItem("Vertex Mode")) {
                }
                if (ImGui::MenuItem("Edge Mode")) {
                }
                if (ImGui::MenuItem("Face Mode")) {
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Modifiers")) {
                if (ImGui::MenuItem("Extrude")) {
                }
                if (ImGui::MenuItem("Boolean")) {
                }
                if (ImGui::MenuItem("Edge Ring")) {
                }
                if (ImGui::MenuItem("Mesh Smooth")) {
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("About")) {
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        ImGui::End();
    }

    const auto viewport = static_cast<ImGuiViewportP *>(static_cast<void *>(ImGui::GetMainViewport()));
    constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;

    if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, ImGui::GetFrameHeight() + 5, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

            ImGui::Button( ICON_FA_ARROWS_ALT, ImVec2(28, 28) );
            ImGui::Button( ICON_FA_EXPAND_ARROWS_ALT, ImVec2(28, 28) );
            ImGui::Button( ICON_FA_UNDO, ImVec2(28, 28) );
            ImGui::PopStyleVar();

            ImGui::Separator();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

            if (ImGui::Button( ICON_FA_GLOBE, ImVec2(28, 28))) {
                scene->setSelectionMode(SelectionMode::OBJECTS);
            }

            if (ImGui::Button( ICON_FA_VECTOR_SQUARE, ImVec2(28, 28))) {
                scene->setSelectionMode(SelectionMode::VERTICES);
            }

            if (ImGui::Button( ICON_FA_DRAW_POLYGON, ImVec2(28, 28))) {
                scene->setSelectionMode(SelectionMode::EDGES);
            }

            if (ImGui::Button( ICON_FA_CLONE, ImVec2(28, 28))) {
                scene->setSelectionMode(SelectionMode::FACES);
            }

            if (ImGui::Button( ICON_FA_IMAGE, ImVec2(28, 28))) {
                scene->texturesEnabled = !scene->texturesEnabled;
            }

            ImGui::PopStyleVar();
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    if (ImGui::BeginViewportSideBar("##MainStatusBar", viewport, ImGuiDir_Down, ImGui::GetFrameHeight() + 5, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text("Happy status bar");
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    ImGui::SetNextWindowPos(ImVec2(0, 60.0f));
    ImGui::SetNextWindowSize(ImVec2(scene->getCamera()->viewWidth, scene->getCamera()->viewHeight - 92.0f), 1);

    if (ImGui::Begin("Render", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar)) {
        window->getSystem<RenderSystem>().update(deltaTime);
        unsigned int renderTexture = window->getSystem<RenderSystem>().renderTexture();

        //ImGui::Image(2, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::GetWindowDrawList()->AddImage(
            reinterpret_cast<ImTextureID>(reinterpret_cast<void *>(renderTexture)),
            ImVec2(0, 0),
            ImVec2(scene->getCamera()->viewWidth, scene->getCamera()->viewHeight),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        // Subscribe to reposition gizmo events. The listener handles positioning.
        EventManager::subscribe<RepositionGizmoEvent, SceneRenderedListener>();

        // Gizmos
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
        ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        const auto& camera = scene->getCamera();
        ImGuizmo::Manipulate(glm::value_ptr(camera->getView()),glm::value_ptr(camera->getProjection()),ImGuizmo::TRANSLATE, ImGuizmo::WORLD,glm::value_ptr(GizmoMatrixState::get()));

        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

