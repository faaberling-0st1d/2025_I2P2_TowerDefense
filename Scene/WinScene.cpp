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
int first_keycode = 1;
int typing_enable = 0;

void WinScene::Initialize() {
    result_outputted = 0;
    first_keycode = 1;
    typing_enable = 0;
    
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));

    // Input box
    textbox_img = new Engine::Image("win/textbox_img.png", halfW, halfH / 4 + 60, 0, 0, 0.5, 0.5);
    textbox_lbl = new Engine::Label("Nickname", "pirulen.ttf", 48, halfW, halfH / 4 + 60, 10, 255, 255, 255, 0.5, 0.5);textbox_img = new Engine::Image("win/textbox_img.png", halfW, halfH / 4 + 60, 0, 0, 0.5, 0.5);
    AddNewObject(textbox_img);
    AddNewObject(textbox_lbl);

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this));
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

void WinScene::BackOnClick() {
    // File write in.
    std::ofstream fout;
    fout.open("Resource/scoreboard.txt", std::fstream::app); // Use appending file output mode!
    if (!fout.fail()) {
        if (!result_outputted) {
            fout.seekp(0, std::ofstream::end);
            std::streamsize fsize = fout.tellp();
            if (fsize != 0) fout << std::endl; // Add a change line character at the beginning if the file is not empty.

            // Getting user's name.
            if (textbox_lbl->Text.empty()) textbox_lbl->Text = "Anonymous"; // In case the user inputted an empty string.
            std::string score_line = (textbox_lbl->Text + incomplete_score_line);
            fout << score_line;
            result_outputted = 1;
        }
        fout.close(); // Save memory!!!

        RemoveObject(textbox_lbl->GetObjectIterator()); // Remove textbox_lbl.
        RemoveObject(textbox_img->GetObjectIterator()); // Remove textbox_img.
    } else {
        std::cout << "[BUG] `scoreboard.txt` not found!" << std::endl;
        exit(1);
    }
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void WinScene::OnMouseDown(int button, int mx, int my) {
    IScene::OnMouseDown(button, mx, my);
    std::cout << "[DEBUGGER] collider->IsPointInRect(Engine::Point(mx, my), textbox_img->Position - textbox_img->Size, textbox_img->Size * 2) = " << collider->IsPointInRect(Engine::Point(mx, my), textbox_img->Position - textbox_img->Size, textbox_img->Size * 2) << std::endl;
    if ((button & 1) && /* If the user clicks left button (mouse) ... */\
        collider->IsPointInRect(Engine::Point(mx, my), textbox_img->Position - textbox_img->Size, textbox_img->Size * 2)) { /* ... and the mouse is inside the textbox_img: */
        
        typing_enable = 1;
        std::cout << "[DEBUGGER] typing_enable = 1" << std::endl;

    } else {
        typing_enable = 0;
        std::cout << "[DEBUGGER] typing_enable = 0" << std::endl;
    }
    
}
void WinScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
}
void WinScene::OnKeyDown(int keycode) {
    std::cout << "[DEBUGGER] OnKeyDown()" << std::endl;
    char c;

    IScene::OnKeyDown(keycode);
    if (typing_enable) {
        if (first_keycode) {
            textbox_lbl->Text = "";
            first_keycode = 0;
        }

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
            if (!textbox_lbl->Text.empty()) textbox_lbl->Text.pop_back();

        } else {
            return;
        }
    }
}