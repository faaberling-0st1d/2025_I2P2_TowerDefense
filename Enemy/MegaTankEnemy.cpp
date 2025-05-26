#include <iostream>
#include <string>
#include <map>
#include <cmath>

#define MAX_DISABLE_TIMESPAN 100

#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include "Enemy/MegaTankEnemy.hpp"
#include "Engine/Sprite.hpp"
#include "Turret/Turret.hpp"
#include "Scene/PlayScene.hpp"

MegaTankEnemy::MegaTankEnemy(int x, int y) : Enemy("play/enemy-6.png", x, y, 40, 10, 10000, 10000) {
}

// Destroy turret within a certain radius
void MegaTankEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    disable_radius += 1 * std::sin(disable_radius_tick / (40 * ALLEGRO_PI));

    Pos_Turret_Map::iterator it = TurretMap.begin();
    Turret *disabled = nullptr;
    for (; it != TurretMap.end(); it++) {
        Engine::Point _dist_pt = it->first - this->Position;
        if (std::sqrt(_dist_pt.x*_dist_pt.x + _dist_pt.y*_dist_pt.y) <= this->disable_radius) {
            it->second->Enabled = false;
            it->second->Tint = al_map_rgba(100, 0, 0, 255);
            it->second->enable_tick = 1;

        } else if (!it->second->Enabled && it->second->enable_tick <= MAX_DISABLE_TIMESPAN) {
            it->second->enable_tick++;

        } else {
            it->second->enable_tick = 0;
            it->second->Tint = al_map_rgba(255, 255, 255, 255);
            it->second->Enabled = true;
        } 
    }
    disable_radius_tick += ALLEGRO_PI;
}

void MegaTankEnemy::Draw() const {
    Sprite::Draw();
    if (PlayScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
    al_draw_filled_circle(Position.x, Position.y, disable_radius, al_map_rgba(255, 0, 0, 40));
}