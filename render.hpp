#include "ecs.hpp"
#include <SFML/Graphics.hpp>

class RenderSystem : ISystem {
    public:
     RenderSystem(sf::RenderTarget& target);
     void update(float) override;
};
