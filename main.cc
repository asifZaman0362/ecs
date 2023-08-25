#include "ecs.hpp"
#include <iostream>


#include "position.hpp"
#include "velocity.hpp"
#include "colorsystem.hpp"
#include "render.hpp"

#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

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

sf::CircleShape rand_circle() {
    float radius = rand() % 20 + 5.0f;
    sf::CircleShape circle(radius);
    return circle;
}

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 640), "ecs");    
    window.setVerticalSyncEnabled(false);
    srand(time(0));
    Coordinator coordinator;
    bool res = 0;
    res = coordinator.LoadSystem(std::make_shared<ColorSystem>());
    res = coordinator.LoadSystem(std::make_shared<PosSys>());
    res = coordinator.LoadSystem(std::make_shared<VelocitySystem>());
    res = coordinator.LoadSystem(std::make_shared<RenderSystem>(&window));
    Registry::RegisterComponent<Color>();
    Registry::RegisterComponent<Position>();
    Registry::RegisterComponent<Velocity>();
    for (int i = 0; i < 20; i++) {
        Entity e = Registry::CreateEntity();
        Registry::AddComponent<Color>(e, rand_col());
        Registry::AddComponent<Position>(e, rand_pos());
        Registry::AddComponent<Velocity>(e, rand_vel());
    }
    bool running = true;
    while (running) {
        sf::Event e{};
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) running = false;
        }
        window.clear(sf::Color::Red);
        coordinator.update(1.0f);
        window.display();
    }
    Coordinator::UnloadSystem<ColorSystem>();
    Coordinator::UnloadSystem<PosSys>();
    Coordinator::UnloadSystem<VelocitySystem>();
    Coordinator::UnloadSystem<RenderSystem>();
    return 0;
}
