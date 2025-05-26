#ifndef MEGATANKENEMY_HPP
#define MEGATANKENEMY_HPP
#include "Enemy.hpp"

class MegaTankEnemy : public Enemy {
    float disable_radius = 100;
    float disable_radius_tick = 0;
public:
    MegaTankEnemy(int x, int y);
    void Update(float deltaTime) override;
    void Draw() const override;
};
#endif // MEGATANKENEMY_HPP