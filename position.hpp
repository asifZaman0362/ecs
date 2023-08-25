#include "ecs.hpp"

struct __attribute__((packed)) Position {
    float x, y, z;
};

class PosSys : public ISystem {
   public:
    void update(float) override;
};
