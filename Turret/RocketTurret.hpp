#ifndef ROCKETTURRET_HPP
#define ROCKETTURRET_HPP
#include "Turret.hpp"

class RocketTurret : public Turret {
public:
    static const int Price;
    RocketTurret(float x, float y);
    void CreateBullet() override;
};
#endif // ROCKETTURRET_HPP
