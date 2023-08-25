#include "ecs.hpp"
#include <SFML/Graphics.hpp>

class RenderSystem : public ISystem {
    public:
     RenderSystem(sf::RenderTarget* target);
     Signature GetSignature() override;
     void update(float) override;
    private:
     sf::RenderTarget* target;
};
