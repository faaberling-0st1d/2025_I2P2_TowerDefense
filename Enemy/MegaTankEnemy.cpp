#include <string>

#include "MegaTankEnemy.hpp"
#include "Scene/PlayScene.hpp"

MegaTankEnemy::MegaTankEnemy(int x, int y) : Enemy("play/enemy-6.png", x, y, 40, 10, 10000, 10000) {
}

// MegaTankEnemy::Update(float deltaTime) {
//     Enemy::Update(deltaTime);

//     // Pick a turret within radius = 3 * BlockSize to destroy!
//     int destroy_r = PlayScene::BlockSize * 3;
//     for (auto turret : PlayScene::TowerGroup->) {
//         Engine::Point dist = this->Position - turret
//         if ()
//     }
// }