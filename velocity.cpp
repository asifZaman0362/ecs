#include "velocity.hpp"
#include "position.hpp"
#include <iostream>

void VelocitySystem::update(float dt) {
    for (auto entity : m_entities) {
        auto vel = Registry::GetComponent<Velocity>(entity);
        auto& pos = Registry::GetComponent<Position>(entity);
        pos.x += vel.x;
        pos.y += vel.y;
    }
}
