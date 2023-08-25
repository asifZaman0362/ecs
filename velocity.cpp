#include "velocity.hpp"
#include "position.hpp"
#include <iostream>

void VelocitySystem::update(float dt) {
    for (auto entity : m_entities) {
        auto vel = Registry::GetComponent<Velocity>(entity);
        auto& pos = Registry::GetComponent<Position>(entity);
        if ((pos.x < 0 && vel.x < 0) || (pos.x > 800 && vel.x > 0)) {
            vel.x *= -1;
        } else if  ((pos.y < 0 && vel.y < 0) || (pos.y > 640 && vel.y > 0)) {
            vel.y *= -1;
        }
        pos.x += vel.x;
        pos.y += vel.y;
    }
}

Signature VelocitySystem::GetSignature() {
    Signature signature;
    signature.set(GetId<Position>());
    signature.set(GetId<Velocity>());
    return signature;
}
