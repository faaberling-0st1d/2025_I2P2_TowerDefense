#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

// File I/O
#include <iostream>
#include <fstream>

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

    // Scoreboard Text
    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 40, halfW, halfH * 1 / 6, 0, 203, 0, 255, 0.5, 0.5));
    std::ifstream file_in; // Input file stream.
    file_in.open("2025_I2P2_TowerDefense/Resource/scoreboard.txt"); // Open the file.
    if (file_in.fail()) {
        std::cout << "[ERROR] 2025_I2P2_TowerDefense/Resource/scoreboard.txt not found!" << std::endl;
        exit(1);
    }
    int h_delta = 60;
    while (!file_in.eof()) {
        std::string name, score;
        file_in >> name >> score;
        AddNewObject(new Engine::Label(name, "pirulen.ttf", 40, halfW - 100, halfH * 1 / 6 + h_delta, 0, 101, 0, 255, 0.5, 0.5));
        AddNewObject(new Engine::Label(score, "pirulen.ttf", 40, halfW + 200, halfH * 1 / 6 + h_delta, 0, 101, 0, 255, 0.5, 0.5));
        h_delta += 40;
    }
    file_in.close(); // Save memory!!!

    // Back button (back to stage-select scene)
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 700, halfH * 3 / 2 - 50, 400, 100);
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 48, halfW - 500, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 300, halfH * 3 / 2 - 50, 400, 100);
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 48, halfW + 500, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
}

void ScoreboardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void ScoreboardScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}