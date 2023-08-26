#include <cstdarg>

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
            pos->x += velocity->x * dt;
            pos->y += velocity->y * dt;
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

int main() {
    coordinator.RegisterComponent<Color>();
    coordinator.RegisterComponent<Velocity>();
    coordinator.RegisterComponent<Position>();
    coordinator.LoadSystem<ColorSystem>();
    coordinator.LoadSystem<VelocitySystem>();
    coordinator.LoadSystem<PrintSystem>();
    for (int i = 0; i < 10; i++) {
        Entity e = coordinator.CreateEntity();
        coordinator.AddComponent<Color>(e, {10, 10, 10});
        coordinator.AddComponent<Position>(e, {10, 10});
        coordinator.AddComponent<Velocity>(e, {10, 10});
    }
    while (true) {
        coordinator.Update(1);
    }
    return 0;
}
