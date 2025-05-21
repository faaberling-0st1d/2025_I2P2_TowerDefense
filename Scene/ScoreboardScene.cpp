#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Scene/PlayScene.hpp"
#include "ScoreboardScene.hpp"
#include "Scene/SettingsScene.hpp" // Added in order to get back from stage select scene.
#include "Scene/StageSelectScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void ScoreboardScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
}

void ScoreboardScene::Terminate() {
    IScene::Terminate();
}

void ScoreboardScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}