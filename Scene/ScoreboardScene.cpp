#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

// File I/O
#include <iostream>
#include <fstream>
#define SCOREBOARD_TXT "Resource/scoreboard.txt"
#define MAX_LINES_PER_PAGE 5

// STL
#include <map>
#include <vector>
#include <list>

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

// Store & Sort the scoreboard.
typedef std::multimap<int, std::string, std::greater<int>> SortingMultimap;
SortingMultimap local_scoreboard; // Use a multimap to sort (score - name).
std::list<Engine::Label *> lbl_list;
int page_head = 0;

void ScoreboardScene::Initialize() {
    // Since the above three objects (local_scoreboard, lbl_list, page_head) is not created by the TAs, ...
    // ... we have to initialize them manually.
    local_scoreboard.clear(); // Clear multimap local_scoreboard.
    lbl_list.clear();         // Clear list lbl_list.
    page_head = 0;            // Set integer page_head = 0;

    std::cout << "[DEBUGGER] Scoreboard Initialized!" << std::endl;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // Scoreboard Text
    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 40, halfW, halfH * 1 / 6, 0, 203, 0, 255, 0.5, 0.5));
    ScoreboardScoreSorter(); // Sort the score records first!
    ScoreboardPrinter(); // Print the scoreboard!

    Engine::ImageButton *btn;
    // PREV PAGE Button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 700, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::PrevPageOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev page", "pirulen.ttf", 40, halfW - 500, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    // NEXT PAGE Button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 300, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::NextPageOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next page", "pirulen.ttf", 40, halfW + 500, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));


    // Back button (back to stage-select scene)
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
}

void ScoreboardScene::Terminate() {
    std::cout << "[DEBUGGER] ScoreboardScene Terminated!" << std::endl;
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void ScoreboardScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

// Prev Page / Next Page Buttons Functions
void ScoreboardScene::PrevPageOnClick() {
    std::cout << "[DEBUGGER] PrevPageOnClick()" << std::endl;
    page_head -= MAX_LINES_PER_PAGE;
    if (page_head < 0) page_head = 0;
    std::cout << "[DEBUGGER] page_head = " << page_head << std::endl;

    ClearScoreboardLabels();
    ScoreboardPrinter();
}

void ScoreboardScene::NextPageOnClick() {
    std::cout << "[DEBUGGER] NextPageOnClick()" << std::endl;
    int orig_page_head = page_head;
    page_head += MAX_LINES_PER_PAGE;
    if (page_head >= local_scoreboard.size()) page_head = orig_page_head;
    std::cout << "[DEBUGGER] page_head = " << page_head << std::endl;

    ClearScoreboardLabels();
    ScoreboardPrinter();
}

// Scoreboard Sorter (the order of scores)
void ScoreboardScene::ScoreboardScoreSorter() {
    local_scoreboard.clear(); // Reset our local scoreboard.

    std::ifstream fin;
    fin.open(SCOREBOARD_TXT);
    if (fin.fail()) {
        std::cout << "[ERROR] File \"" << SCOREBOARD_TXT << "\" not found! (MODE: file input)" << std::endl;
        exit(1);
    } else {
        std::cout << "[LOG] File \"" << SCOREBOARD_TXT << "\" opened successfully! Start sorting..." << std::endl;

        // Check if the file is empty.
        fin.seekg(0, std::ifstream::end); // Move the file cursor to the end of the file
        std::streamsize fsize = fin.tellg();
        std::cout << "[DEBUGGER] fsize = " << fsize << std::endl;
        if (fsize == 0) {
            fin.close(); // Close the file. (Saving memory!)
            return; // Do nothing.
        }
        
        fin.seekg(0); // Move the file cursor to the head of the file again!
        while (!fin.eof()) {
            std::string name; int score;
            fin >> name >> score;
            local_scoreboard.insert({score, name});
        }
        fin.close(); // Save memory!

        // Rewrite the file.
        std::ofstream out;
        out.open(SCOREBOARD_TXT);
        if (out.fail()) {
            std::cout << "[ERROR] File \"" << SCOREBOARD_TXT << "\" not found! (MODE: file output)" << std::endl;
            exit(1);
        } else {
            std::cout << "[LOG] File \"" << SCOREBOARD_TXT << "\" opened successfully! Start rewriting..." << std::endl;
            SortingMultimap::iterator it = local_scoreboard.begin();
            out << it->second << " " << it->first; // Output format: `it->second(name) it->first(score)`
            std::cout << "[DEBUGGER]" << std::endl; // Debugger.
            std::cout << it->second << " " << it->first; // Debugger.
            it++;
            for (; it != local_scoreboard.end(); it++) { // Please remember to move the iterator forward...
                out << "\n" << it->second << " " << it->first; // Output format: `<endl>it->second(name) it->first(score)`
                std::cout << "\n" << it->second << " " << it->first; // Debugger.
            }
            std::cout << std::endl; // Debugger.
            out.close(); // Save memory!
        }
    }
}

// Scoreboard Printer
void ScoreboardScene::ScoreboardPrinter() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    
    std::ifstream file_in; // Input file stream.
    file_in.open(SCOREBOARD_TXT); // Open the file.
    if (file_in.fail()) {
        std::cout << "[ERROR] File \"" << SCOREBOARD_TXT << "\" not found!" << std::endl;
        exit(1);
    }
    int h_delta = 60;
    for (int line_i = 0;\
            (line_i < page_head + MAX_LINES_PER_PAGE && !file_in.eof()); line_i++) {
        std::string name, score;
        file_in >> name >> score;
        if (page_head <= line_i && line_i < page_head + MAX_LINES_PER_PAGE) { // The lines we want!
            Engine::Label *lbl;
            lbl = new Engine::Label(name, "pirulen.ttf", 40, halfW - 100, halfH * 1 / 6 + h_delta, 0, 101, 0, 255, 0.5, 0.5); // Name label.
            this->AddNewObject(lbl);
            lbl_list.push_back(lbl); // Add this name label into the label list.
            lbl = new Engine::Label(score, "pirulen.ttf", 40, halfW + 200, halfH * 1 / 6 + h_delta, 0, 101, 0, 255, 0.5, 0.5); // Score label.
            this->AddNewObject(lbl);
            lbl_list.push_back(lbl); // Add this score label into the label list.
            h_delta += 40; // Increase the height delta if we really printed something.
        }
    }
    file_in.close(); // Save memory!!!
}

void ScoreboardScene::ClearScoreboardLabels() {
    for (auto lbl : lbl_list) {
        RemoveObject(lbl->GetObjectIterator());
    }
    lbl_list.clear();
}