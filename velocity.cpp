#include "velocity.hpp"
#include <iostream>

void VelocitySystem::update(float dt) {
    std::cout << "Velocity Id: " << GetId<Velocity>() << std::endl;
}
