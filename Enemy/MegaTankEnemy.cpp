#include <string>
#include <map>
#include <cmath>

#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include "MegaTankEnemy.hpp"
#include "Scene/PlayScene.hpp"

MegaTankEnemy::MegaTankEnemy(int x, int y) : Enemy("play/enemy-6.png", x, y, 40, 10, 10000, 10000) {
}

// Destroy turret within a certain radius
void MegaTankEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    destroy_radius += 1 * std::sin(destroy_radius_tick / (40 * ALLEGRO_PI));

    Pos_Turret_Map::iterator it = TurretMap.begin();
    Turret *del = nullptr;
    for (; it != TurretMap.end(); it++) {
        Engine::Point _dist_pt = it->first;
        if (std::sqrt(_dist_pt.x*_dist_pt.x + _dist_pt.y*_dist_pt.y) <= destroy_radius) {
            del = it->second;
            TurretMap.erase(it); // Remove the turret from my turret map.
            // PlayScene::TowerGroup->RemoveObject(del->GetObjectIterator());
        }
    }
    destroy_radius_tick += ALLEGRO_PI;
}

void MegaTankEnemy::Draw() const {
    Sprite::Draw();
    if (PlayScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
    al_draw_filled_circle(Position.x, Position.y, destroy_radius, al_map_rgba(255, 0, 0, 40));
}