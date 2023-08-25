#include "ecs.hpp"

struct Velocity {
    float x, y;
};

class VelocitySystem : public ISystem {
   public:
    void update(float) override;
};
