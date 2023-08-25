#include "colorsystem.hpp"
#include <iostream>

void ColorSystem::update(float dt) {
    std::cout << "Color comp id: " << GetId<Color>() << "\n";
}
