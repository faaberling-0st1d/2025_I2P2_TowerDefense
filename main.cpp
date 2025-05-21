// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/ScoreboardScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"


int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // TODO HACKATHON-2 (2/3): Register Scenes here
	game.AddNewScene("scoreboard-scene", new ScoreboardScene()); // Creare scene "scoreboard-scene" (TODO PROJECT 2)
	game.AddNewScene("settings", new SettingsScene()); // Create scene "settings".
    game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
	game.AddNewScene("start", new StartScene()); // Create scene "start".

    // TODO HACKATHON-1 (1/1): Change the start scene
	game.Start("start", 60, 1600, 832);          // Start from scene "start".
	return 0;
}
