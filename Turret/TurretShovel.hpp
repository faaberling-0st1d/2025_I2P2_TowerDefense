#ifndef TURRETSHOVEL_HPP
#define TURRETSHOVEL_HPP
#include "Turret.hpp"

class TurretShovel : public Turret {
public:
    static const int Price;
    TurretShovel(float x, float y);
    void CreateBullet() override;
};
#endif // TURRETSHOVEL_HPP
