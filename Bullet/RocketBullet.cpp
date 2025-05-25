#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Collider.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "RocketBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

class Turret;

RocketBullet::RocketBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent) : Bullet("play/bullet-3.png", 400, 1000, position, forwardDirection, rotation + ALLEGRO_PI / 2, parent) {
}
void RocketBullet::OnExplode(Enemy *enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 10);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), enemy->Position.x, enemy->Position.y));
}
void RocketBullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene *scene = getPlayScene();
    // Can be improved by Spatial Hash, Quad Tree, ...
    // However simply loop through all enemies is enough for this program.
    for (auto &it : scene->EnemyGroup->GetObjects()) {
        Enemy *enemy = dynamic_cast<Enemy *>(it);
        if (!enemy->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
            OnExplode(enemy);
            enemy->Hit(damage);
            if (enemy->speed > 0) enemy->speed--;
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            // AudioHelper::PlayAudio("452645__kyles__ice-grinding-cracking-freezing-designed.wav");
            return;
        }
    }
    // Check if out of boundary.
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize()))
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}
 