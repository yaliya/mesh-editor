#include "src/window.h"

#include "src/systems/gui_system.h"
#include "src/systems/input_system.h"
#include "src/systems/render_system.h"
#include "src/managers/asset_manager.h"
#include "src/managers/scene_manager.h"

int main() {
    const auto scene = std::make_shared<SceneManager>();
    const auto window = std::make_shared<Window>(1280, 720);

    window->setScene(scene);
    window->addSystem<GUISystem>();
    window->addSystem<InputSystem>();
    window->addSystem<RenderSystem>();

    while (!window->shouldClose()) {
        window->update();
    }

    return 0;
}
