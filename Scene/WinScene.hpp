#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

extern std::string incomplete_score_line; // Incomplete score line (without user-entered name)

class WinScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;

public:
    explicit WinScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    // void OnMouseDown(int button, int mx, int my) override;
    // void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void BackOnClick(int stage);
    void UpdateTextbox(int keycode);
};

#endif   // WINSCENE_HPP
