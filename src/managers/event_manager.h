#pragma once

#include <vector>
#include <typeindex>
#include <functional>
#include <unordered_map>

#include "../core/event.h"
#include "../util/singleton.h"

class EventManager : Singleton {
public:
    using EventListener = std::function<void(const Event&)>;
private:
    std::unordered_map<std::type_index, std::vector<EventListener>> listeners;
public:
    template<typename EventType>
    static void subscribe(const EventListener& listener) {
        instance<EventManager>().listeners[typeid(EventType)].push_back(listener);
    }

    template<typename EventType, typename ListenerType>
    static void subscribe() {
        instance<EventManager>().listeners[typeid(EventType)].push_back(ListenerType::listen);
    }

    template<typename EventType, typename... Args>
    static void dispatch(Args&&... args) {
        auto self = instance<EventManager>();
        EventType event(std::forward<Args>(args)...);

        if (const auto it = self.listeners.find(typeid(EventType)); it != self.listeners.end()) {
            for (const auto& listener : it->second) {
                listener(event);
            }
        }
    }
};
