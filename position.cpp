#include "position.hpp"

#include <iostream>

void PosSys::update(float dt) {
}

Signature PosSys::GetSignature() {
    Signature signature;
    signature.set(GetId<Position>(), true);
    return signature;
}
