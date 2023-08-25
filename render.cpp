#include "render.hpp"

#include "colorsystem.hpp"
#include "position.hpp"

static const sf::Color colors[] = {sf::Color::Red, sf::Color::Black,
                                   sf::Color::White, sf::Color::Blue,
                                   sf::Color::Green};

RenderSystem::RenderSystem(sf::RenderTarget* target) { this->target = target; }

void RenderSystem::update(float) {
    for (auto entity : m_entities) {
        auto [x, y, z] = Registry::GetComponent<Position>(entity);
        auto color = colors[Registry::GetComponent<Color>(entity).color];
        auto shape = Registry::GetComponent<sf::CircleShape>(entity);
        shape.setPosition(x, y);
        shape.setFillColor(color);
        target->draw(shape);
    }
}

Signature RenderSystem::GetSignature() {
    Signature signature;
    signature.set(GetId<Position>(), true);
    signature.set(GetId<Color>(), true);
    return signature;
}
