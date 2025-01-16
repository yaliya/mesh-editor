#pragma once

#include <memory>
#include <stack>

#include "../core/command.h"

class CommandManager {
    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;
    std::unique_ptr<Command> lastCommand;
public:
    void execute(std::unique_ptr<Command> command, bool repeat);
    void loop(std::unique_ptr<Command> command, bool flag);
    void done();
    void undo();
    void redo();
};
