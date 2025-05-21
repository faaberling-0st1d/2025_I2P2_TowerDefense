#ifndef ScoreboardScene_HPP
#define ScoreboardScene_HPP
#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class ScoreboardScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick();
};

#endif   // ScoreboardScene_HPP