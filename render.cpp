#include "render.hpp"

#include "colorsystem.hpp"
#include "position.hpp"

static const sf::Color colors[] = {sf::Color::Red, sf::Color::Black,
                                   sf::Color::White, sf::Color::Blue,
                                   sf::Color::Green};

RenderSystem::RenderSystem(sf::RenderTarget* target) { this->target = target; }

void RenderSystem::update(float) {
    for (auto entity : m_entities) {
        auto color = colors[Registry::GetComponent<Color>(entity).color];
    }
}
