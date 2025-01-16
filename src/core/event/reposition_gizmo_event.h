#pragma once

#include "../event.h"

/**
 * This event should be fired whenever we want to reposition the gizmo.
 * The calculation of the position happens on the RepositionGizmoListener event listener.
 */
struct RepositionGizmoEvent final : Event {
    SelectionMode mode;
    explicit RepositionGizmoEvent(const SelectionMode& mode) : mode(mode) {};
};
