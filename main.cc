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
    Registry registry;
    bool res = 0;
    res = coordinator.LoadSystem(std::make_shared<ColorSystem>());
    res = coordinator.LoadSystem(std::make_shared<PosSys>());
    res = coordinator.LoadSystem(std::make_shared<VelocitySystem>());
    registry.RegisterComponent<Color>();
    registry.RegisterComponent<Position>();
    registry.RegisterComponent<Velocity>();
    for (int i = 0; i < 20; i++) {
        Entity e = registry.CreateEntity();
        registry.AddComponent<Color>(e, rand_col());
        registry.AddComponent<Position>(e, rand_pos());
        registry.AddComponent<Velocity>(e, rand_vel());
    }
    while (true) {
        coordinator.update(1.0f);
    }
    return 0;
}
