#include "ecs.hpp"

struct Color {
    enum ColorEnum { Red = 0, Black, White, Blue, Green } color;
};

class ColorSystem : public ISystem {
   public:
    void update(float) override;
    Signature GetSignature() override;
};
