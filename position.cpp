#include "position.hpp"
#include <iostream>

void PosSys::update(float dt) {
    std::cout << "Pos Id: " << GetId<Position>() << "\n";
}
