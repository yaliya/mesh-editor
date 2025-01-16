#pragma once

class Singleton {
protected:
    Singleton() = default;

    template <typename T>
    static T& instance() {
        static T instance;
        return instance;
    }
};