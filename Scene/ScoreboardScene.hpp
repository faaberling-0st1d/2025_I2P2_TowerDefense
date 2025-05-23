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
    void Update(); // To update page for scoreboard.
    void Terminate() override;
    void BackOnClick();
    void PrevPageOnClick(); // Prev Page Button Function
    void NextPageOnClick(); // Next Page Button Function

    // For scoreboard file management
    void ScoreboardScoreSorter();
    void ScoreboardPrinter();

    void ClearScoreboardLabels();
};

#endif   // ScoreboardScene_HPP