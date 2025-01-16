#pragma once

class Command {
public:
    bool stackable = false;
    virtual bool execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual void done() {};
    virtual ~Command() = default;
};