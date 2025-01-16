#include "command_manager.h"

#include <vector>

void CommandManager::execute(std::unique_ptr<Command> command, const bool repeat) {
    // if (!repeat) {
    //     if (!command->stackable) {
    //         // Remove all non-stackable commands of the same type from the undo stack
    //         std::vector<std::unique_ptr<Command>> tempStack;
    //
    //         while (!undoStack.empty()) {
    //             if (typeid(*undoStack.top()) == typeid(*command) && !undoStack.top()->stackable) {
    //                 undoStack.top()->undo();
    //                 redoStack.push(std::move(undoStack.top()));
    //                 undoStack.pop();
    //             } else {
    //                 // Temporarily store other commands
    //                 tempStack.push_back(std::move(undoStack.top()));
    //                 undoStack.pop();
    //             }
    //         }
    //
    //         // Restore remaining commands to the undo stack
    //         for (auto it = tempStack.rbegin(); it != tempStack.rend(); ++it) {
    //             undoStack.push(std::move(*it));
    //         }
    //     }
    // }

    if (command->execute()) {
        undoStack.push(std::move(command));

        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }
}
// New method for handling continuous command execution
void CommandManager::loop(std::unique_ptr<Command> command, const bool flag) {
    if (flag) {
        if (!lastCommand) {
            lastCommand = std::move(command);
        } else {
            lastCommand->execute();
        }
    }
}

void CommandManager::done() {
    if (lastCommand) {
        lastCommand->done();
        undoStack.push(std::move(lastCommand));

        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }
}


void CommandManager::undo() {
    if (!undoStack.empty()) {
        undoStack.top()->undo();
        redoStack.push(std::move(undoStack.top()));
        undoStack.pop();
    }
}

void CommandManager::redo() {
    if (!redoStack.empty()) {
        redoStack.top()->redo();
        undoStack.push(std::move(redoStack.top()));
        redoStack.pop();
    }
}