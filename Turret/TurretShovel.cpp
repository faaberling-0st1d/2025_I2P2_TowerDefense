#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/RocketBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "TurretShovel.hpp"
#include "Scene/PlayScene.hpp"

const int TurretShovel::Price = 50;
TurretShovel::TurretShovel(float x, float y)
    : Turret("play/shovel-base.png", "play/shovel.png", x, y, 0, Price, 0.5) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void TurretShovel::CreateBullet() {
    // No bullet.
}
