#include "ecs.hpp"

Registry::Registry() {
    for (size_t i = 0; i < MAX_ENTITIES; i++) {
        available_entities.push_back(i);
    }
}

void Registry::EntityDestroyed(Entity e) {
    for (auto list : component_lists) {
        list.second->OnEntityDelete(e);
    }
}

Entity Registry::CreateEntity() {
    if (available_entities.size() != 0) {
        auto id = available_entities.front();
        available_entities.pop_front();
        return id;
    }
    static_assert(1, "Ran out of entities!");
    return 0;
}

void Coordinator::update(float dt) {
    auto size = systems.size();
    for (auto system : systems) {
        system.second->update(dt);
    }
}
