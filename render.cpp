#include "render.hpp"
#include "colorsystem.hpp"
#include "position.hpp"

void RenderSystem::update(float) {
    auto colors = Registry::GetComponentArray<Color>();
    auto positions = Registry::GetComponentArray<Position>();
}
