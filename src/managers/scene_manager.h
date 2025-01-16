#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "../camera.h"
#include "../primitives/grid.h"
#include "../types/model.h"

enum SelectionMode {
    NONE,
    OBJECTS,
    VERTICES,
    EDGES,
    FACES
};

class SceneManager {
    std::shared_ptr<Grid> grid;
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<Model>> models;

    unsigned int renderedTexture{};
    SelectionMode selectionMode = SelectionMode::NONE;
public:
    SceneManager();
    ~SceneManager();

    bool texturesEnabled{};

    void clear();
    void setViewport(int width, int height) const;

    void addCamera(const std::shared_ptr<Camera>& camera);
    std::shared_ptr<Camera> getCamera();

    void addGrid(const std::shared_ptr<Grid>& grid);
    std::shared_ptr<Grid> getGrid();

    void addModel(const std::shared_ptr<Model>& model);
    std::vector<std::shared_ptr<Model>>& getModels();

    void setSelectionMode(SelectionMode mode);
    [[nodiscard]] SelectionMode getSelectionMode() const;
};
