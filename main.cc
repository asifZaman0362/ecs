#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "ecs.hpp"

struct Color {
    float r, g, b;
};

struct Position {
    float x, y;
};

struct Velocity {
    float x, y;
};

Coordinator coordinator;

Signature SignatureHelper(int count, ...) {
    va_list args;
    va_start(args, count);
    Signature signature;
    for (int i = 0; i < count; i++) {
        size_t id = va_arg(args, size_t);
        signature.set(id, true);
    }
    va_end(args);
    return signature;
}

class ColorSystem : public ISystem {
   public:
    Signature GetSignature() override {
        return SignatureHelper(2, GetComponentId<Color>(),
                               GetComponentId<Velocity>());
    }

    void Update(float) override {
        for (auto& entity : m_entities) {
            auto velocity = coordinator.GetComponent<Velocity>(entity);
            assert(velocity != nullptr);
            auto color = coordinator.GetComponent<Color>(entity);
            assert(color != nullptr);
            color->r = (velocity->x * velocity->y);
            color->g = velocity->x;
            color->b = velocity->y;
        }
    }
};

class VelocitySystem : public ISystem {
   public:
    Signature GetSignature() override {
        return SignatureHelper(2, GetComponentId<Velocity>(),
                               GetComponentId<Position>());
    }
    void Update(float dt) override {
        for (auto entity : m_entities) {
            Velocity* velocity = coordinator.GetComponent<Velocity>(entity);
            assert(velocity != nullptr);
            Position* pos = coordinator.GetComponent<Position>(entity);
            assert(pos != nullptr);
            if ((pos->x < -100 && velocity->x < 0) || (pos->x > 2000 && velocity->x > 0)) {
                velocity->x *= -1;
            }
            if ((pos->y < -100 && velocity->y < 0) || (pos->y > 1300 && velocity->y > 0)) {
                velocity->y *= -1;
            }
            //pos->x = int(pos->x + velocity->x * dt) % 1920;
            //pos->y = int(pos->y + velocity->y * dt) % 1080;
            pos->x += velocity->x;
            pos->y += velocity->y;
        }
    }
};

#include "logger.hpp"
using namespace zifmann::logger;

class PrintSystem : public ISystem {
   public:
    Signature GetSignature() override {
        return SignatureHelper(3, GetComponentId<Color>(),
                               GetComponentId<Velocity>(),
                               GetComponentId<Position>());
    }
    void Update(float dt) override {
        timer += dt;
        if (timer > 60.0f) {
            for (auto entity : m_entities) {
                auto color = coordinator.GetComponent<Color>(entity);
                auto velocity = coordinator.GetComponent<Velocity>(entity);
                auto pos = coordinator.GetComponent<Position>(entity);
                log_debug(
                    "\nEntity #%lu:\n\tPosition: %f %f\n\tVelocity: %f "
                    "%f\n\tColor: %f %f %f",
                    entity, pos->x, pos->y, velocity->x, velocity->y, color->r,
                    color->g, color->b);
            }
            timer = 0;
        }
    }

   private:
    float timer = 0;
};

#include <SFML/Graphics.hpp>
sf::RenderWindow window;

class Renderer : public ISystem {
   public:
    Signature GetSignature() override {
        return SignatureHelper(3, GetComponentId<Color>(),
                               GetComponentId<Position>(),
                               GetComponentId<sf::CircleShape>());
    }
    void Update(float) override {
        for (auto entity : m_entities) {
            auto circle = coordinator.GetComponent<sf::CircleShape>(entity);
            auto pos = coordinator.GetComponent<Position>(entity);
            auto color = coordinator.GetComponent<Color>(entity);
            circle->setPosition(pos->x, pos->y);
            window.draw(*circle);
        }
    }
};

#include <random>

// Function to generate random numbers within a range [min, max]
int generateRandomNumber(int min, int max) {
    std::random_device rd;  // Seed generator
    std::mt19937 mt(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<int> dist(min, max);  // Distribution

    return dist(mt);  // Generate and return a random number
}

sf::CircleShape random_circle() {
    float radius = generateRandomNumber(2, 25);
    sf::CircleShape circle(radius);
    uint8_t r = generateRandomNumber(0, 255);
    uint8_t g = generateRandomNumber(0, 255);
    uint8_t b = generateRandomNumber(0, 255);
    circle.setFillColor(sf::Color{r, g, b, 255});
    return circle;
}

Position random_position() {
    float x = generateRandomNumber(0, 1920);
    float y = generateRandomNumber(0, 1080);
    return { x, y };
}

Velocity random_velocity() {
    float x = generateRandomNumber(10, 20);
    float y = generateRandomNumber(10, 20);
    if (generateRandomNumber(1, 10) > 5) {
        x *= -1;
    }
    if (generateRandomNumber(1, 10) > 5) {
        y *= -1;
    }
    return { x, y };
}

int main() {
    sf::Clock clock;
    sf::Text m_text;
    sf::Font m_sans;
    m_sans.loadFromFile("font.ttf");
    m_text.setFont(m_sans);
    m_text.setString("FPS: ");
    m_text.setFillColor(sf::Color::White);
    window.create(sf::VideoMode::getDesktopMode(), "simplecs", sf::Style::Fullscreen);
    coordinator.RegisterComponent<Color>();
    coordinator.RegisterComponent<Velocity>();
    coordinator.RegisterComponent<Position>();
    coordinator.RegisterComponent<sf::CircleShape>();
    coordinator.LoadSystem<ColorSystem>();
    coordinator.LoadSystem<VelocitySystem>();
    //coordinator.LoadSystem<PrintSystem>();
    coordinator.LoadSystem<Renderer>();
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity e = coordinator.CreateEntity();
        coordinator.AddComponent<Color>(e, {10, 10, 10});
        coordinator.AddComponent<Position>(e, random_position());
        coordinator.AddComponent<Velocity>(e, random_velocity());
        coordinator.AddComponent<sf::CircleShape>(e, random_circle());
    }
    bool running = true;
    char fpsstr[32] = "FPS: ";
    while (running) {
        sf::Event e{};
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) running = false;
        }
        float time = clock.restart().asSeconds();
        if (time == 0) time = 0.01;
        float fps = 1 / time;
        sprintf(fpsstr, "FPS: %f", fps);
        m_text.setString(fpsstr);
        window.clear(sf::Color::Black);
        coordinator.Update(clock.restart().asSeconds());
        window.draw(m_text);
        window.display();
    }
    return 0;
}
