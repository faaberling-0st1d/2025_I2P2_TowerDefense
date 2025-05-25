#include <functional>
#include <string>
#include <iostream>

// File IO
#include <fstream>
// C++ Date Time
#include <ctime>
#include <chrono>

#include "Engine/AudioHelper.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"

std::string incomplete_score_line = ""; // The declaration of incomplete_score_line

Engine::Collider *collider;
Engine::Image *textbox_img;
Engine::Label *textbox_lbl;
int result_outputted = 0; // The flag - whether the textbox has been used.

void WinScene::Initialize() {
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));

    // Input box
    // textbox_img = new Engine::Image("play/tool-base.jpg", halfW, halfH + 50, 0, 0, 0.5, 0.5);
    textbox_lbl = new Engine::Label("Nickname", "pirulen.ttf", 48, halfW, halfH / 4 + 50, 88, 88, 88, 255, 0.5, 0.5);
    // AddNewObject(textbox_img);
    AddNewObject(textbox_lbl);

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("win.wav");
}
void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }   
}
void WinScene::BackOnClick(int stage) {

    // File write in.
    std::ofstream fout;
    fout.open("Resource/scoreboard.txt", std::fstream::app); // Use appending file output mode!
    if (!fout.fail()) {
        if (!result_outputted) {
            fout.seekp(0, std::ofstream::end);
            std::streamsize fsize = fout.tellp();
            if (fsize != 0) fout << std::endl; // Add a change line character at the beginning if the file is not empty.

            // Getting user's name.
            std::string score_line = (textbox_lbl->Text + incomplete_score_line);
            fout << score_line;
            result_outputted = 1;
        }
        fout.close(); // Save memory!!!
    } else {
        std::cout << "[BUG] `scoreboard.txt` not found!" << std::endl;
        exit(1);
    }
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void WinScene::OnKeyDown(int keycode) {
    char c;

    IScene::OnKeyDown(keycode);
    if (ALLEGRO_KEY_A <= keycode && keycode <= ALLEGRO_KEY_Z) {
        c = keycode - ALLEGRO_KEY_A + 'a';
        textbox_lbl->Text += c;

    } else if (keycode == ALLEGRO_KEY_MINUS) {
        c = '_';
        textbox_lbl->Text += c;

    } else if (ALLEGRO_KEY_0 <= keycode && keycode <= ALLEGRO_KEY_9) {
        c = keycode - ALLEGRO_KEY_0 + '0';
        textbox_lbl->Text += c;

    } else if (keycode == ALLEGRO_KEY_BACKSPACE) {
        textbox_lbl->Text.pop_back();

    } else {
        return;
    }
}