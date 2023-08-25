#include "ecs.hpp"
#include <iostream>


#include "position.hpp"
#include "velocity.hpp"
#include "colorsystem.hpp"

#include <cstdlib>
#include <ctime>

Color rand_col() {
    int x = rand() % 4;
    return Color { .color = (Color::ColorEnum) x };
}

Position rand_pos() {
    float x = (rand() % 100) / 10.0;
    float y = (rand() % 100) / 10.0;
    float z = (rand() % 100) / 10.0;
    return { x, y, z };
}

Velocity rand_vel() {
    float x = (rand() % 100) / 10.0;
    float y = (rand() % 100) / 10.0;
    return { x, y };
}

int main() {
    srand(time(0));
    Coordinator coordinator;
    bool res = 0;
    res = coordinator.LoadSystem(std::make_shared<ColorSystem>());
    res = coordinator.LoadSystem(std::make_shared<PosSys>());
    res = coordinator.LoadSystem(std::make_shared<VelocitySystem>());
    Registry::RegisterComponent<Color>();
    Registry::RegisterComponent<Position>();
    Registry::RegisterComponent<Velocity>();
    for (int i = 0; i < 20; i++) {
        Entity e = Registry::CreateEntity();
        Registry::AddComponent<Color>(e, rand_col());
        Registry::AddComponent<Position>(e, rand_pos());
        Registry::AddComponent<Velocity>(e, rand_vel());
    }
    while (true) {
        coordinator.update(1.0f);
    }
    return 0;
}
