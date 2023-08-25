#include "colorsystem.hpp"

#include <iostream>

void ColorSystem::update(float dt) {
}

Signature ColorSystem::GetSignature() {
    Signature signature;
    signature.set(GetId<Color>(), true);
    return signature;
}
