#include "ecs.hpp"

std::array<size_t, MAX_COMPONENTS> Registry::registered_components;
std::unordered_map<size_t, std::shared_ptr<IComponentArray>> Registry::component_lists;
std::deque<Entity> Registry::available_entities;
std::unordered_map<Entity, Signature> Registry::entity_signatures;

std::unordered_map<size_t, std::shared_ptr<ISystem>> Coordinator::systems;
std::unordered_map<size_t, Signature> Coordinator::system_signatures;

void ISystem::AddEntity(Entity e) { m_entities.insert(e); }

void ISystem::RemoveEntity(Entity e) { m_entities.erase(e); }

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

void Coordinator::EntityUpdated(Entity e, Signature new_signature,
                                Signature old) {
    for (auto entry : system_signatures) {
        if ((entry.second & new_signature) == new_signature) {
            systems[entry.first]->AddEntity(e);
        } else if ((entry.second & old) != old) {
            systems[entry.first]->RemoveEntity(e);
        }
    }
}

void Coordinator::OnEntityDestroyed(Entity e) {
    for (auto entry : systems) {
        entry.second->RemoveEntity(e);
    }
}
