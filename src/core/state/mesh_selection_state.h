#pragma once

#include "../../types/mesh.h"
#include "../../util/singleton.h"

// struct MeshSelectionState final : State {
//     // Keeps a reference to the selected mesh
//     std::optional<std::reference_wrapper<Mesh>> mesh;
//     explicit MeshSelectionState(const std::optional<std::reference_wrapper<Mesh>>& mesh) : mesh(mesh) {}
// };

class MeshSelectionState : Singleton {
    Mesh* mesh = nullptr;

public:
    static void set(Mesh* mesh) {
        instance<MeshSelectionState>().mesh = mesh;
    }

    static Mesh* get() {
        return instance<MeshSelectionState>().mesh;
    }

    static void clear() {
        instance<MeshSelectionState>().mesh = nullptr;
    }
};
