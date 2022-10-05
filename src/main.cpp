// Copyright © 2022 Daylon Wilkins & James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <raylib.h>
#include <gui/gui.h>
#include <model/manager.h>
#include <level/level.h>
#include <render/render.h>
#include <stats/frame.h>
#include <iomanip>
#include <sstream>

#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif // PLATFORM_WEB

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_DESKTOP
#define GLSL_VERSION 100
#endif // PLATFORM_DESKTOP

#if defined(PLATFORM_WEB)
std::string assetPrefix = "";
#else
std::string assetPrefix = "../";
#endif // PLATFORM_WEB

#ifdef _WIN32
#include <hideconsole.h>

#endif //_WIN32

void UpdateDrawFrame(void);

// Globals
gui::DrawRect screenRect;
mouse::Info mouseInfo;
std::unique_ptr<gui::Component> centerDot;
std::unique_ptr<level::Level> playableLevel;
std::unique_ptr<gui::Component> menu;
std::unique_ptr<gui::Component> paused;
std::unique_ptr<gui::Component> gameOverScreen;

int main(void) {
#ifdef _WIN32
	HideConsole();
#endif //_WIN32

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	screenRect = gui::DrawRect{0, 0, 1280, 720};
	InitWindow((int)screenRect.ContainerWidth, (int)screenRect.ContainerHeight, "10 Seconds Till Death");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	SetExitKey(0);
	model::manager::Load();
	gui::fontmanager::Load();
	mouseInfo = mouse::Info{};
	const float centerDotSize = 0.008f;
	centerDot = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({
			gui::NewHorizontalPanel({})->SetColor({0, 0, 0, 255})->
				SetXScale((screenRect.ContainerHeight / screenRect.ContainerWidth) * centerDotSize)->SetYScale(centerDotSize)
		})->SetAlignment(gui::Alignment::Center)
	);

	playableLevel = level::GetLevel1();
	menu = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({
			gui::NewHorizontalPanel({
				gui::NewDynamicLabel("Total: ##:##:##.###", [&]()->std::string {
					double totalTime = playableLevel->GetTotalTime();
					int hours = (int)(totalTime / (3600.0));
					int minutes = (int)(totalTime / 60.0) % 60;
					int seconds = (int)totalTime % 60;
					int ms = (int)((long long)(totalTime * 1000.0) % 1000);
					std::ostringstream os;
					os << " Total: ";
					os << std::setfill('0') << std::setw(2) << hours << ":";
					os << std::setfill('0') << std::setw(2) << minutes << ":";
					os << std::setfill('0') << std::setw(2) << seconds << ".";
					os << std::setfill('0') << std::setw(3) << ms;
					return os.str();
				})->SetColor({0, 0, 0, 255})->SetXScale(0.3f),
				gui::NewDynamicLabel("", [&]()->std::string {
					return "Score: " + std::to_string(playableLevel->GetScore());
				})->SetColor({0, 0, 0, 255})->SetXScale(0.3f),
				gui::NewDynamicLabel("Next Check: #.###", [&]()->std::string {
					float timer = playableLevel->GetDeathTimer();
					int seconds = (int)timer % 60;
					int ms = (int)(timer * 1000.0) % 1000;
					std::ostringstream os;
					os << "Next Check: ";
					os << std::setfill('0') << std::setw(1) << seconds << ".";
					os << std::setfill('0') << std::setw(3) << ms;
					return os.str();
				})->SetColor({0, 0, 0, 255})->SetXScale(0.3f)
			})->SetYScale(0.05f)->SetAlignment(gui::Alignment::Justified),
			gui::NewHorizontalPanel({
				gui::NewProgressBar([&]()->float {
					return playableLevel->GetPlayer()->GetHealth() / playableLevel->GetPlayer()->GetMaxHealth();
				}, {
					gui::NewDynamicLabel("", [&]()->std::string {
						return std::to_string((int)playableLevel->GetPlayer()->GetHealth()) + "/" + std::to_string((int)playableLevel->GetPlayer()->GetMaxHealth());
					})->SetColor({255, 255, 255, 255}),
				})->SetXScale(0.48f)->SetYScale(0.8f)->SetColor({220, 40, 40, 255}),
				gui::NewProgressBar([&]()->float {
					return playableLevel->GetPlayer()->GetAmmo() / playableLevel->GetPlayer()->GetMaxAmmo();
				}, {
					gui::NewDynamicLabel("", [&]()->std::string {
						return std::to_string((int)playableLevel->GetPlayer()->GetAmmo()) + "/" + std::to_string((int)playableLevel->GetPlayer()->GetMaxAmmo());
					})->SetColor({255, 255, 255, 255}),
				})->SetXScale(0.48f)->SetYScale(0.8f)->SetColor({190, 190, 40, 255})
			})->SetYScale(0.1f)->SetColor({0, 0, 0, 200}),
		})->SetAlignment(gui::Alignment::Justified)
	);
	paused = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({
			gui::NewLabel("PAUSED")->SetYScale(0.3f),
			gui::NewLabel("Press 'P' to resume")->SetYScale(0.14f)
		})->SetAlignment(gui::Alignment::Center)
	);
	gameOverScreen = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({
			gui::NewLabel("GAME OVER")->SetYScale(0.3f)->SetColor({255, 255, 255, 255}),
			gui::NewDynamicLabel("", [&]()->std::string {
				double totalTime = playableLevel->GetTotalTime();
				int hours = (int)(totalTime / (3600.0));
				int minutes = (int)(totalTime / 60.0) % 60;
				int seconds = (int)totalTime % 60;
				std::ostringstream os;
				os << "Play Time: ";
				os << std::setfill('0') << std::setw(2) << hours << ":";
				os << std::setfill('0') << std::setw(2) << minutes << ":";
				os << std::setfill('0') << std::setw(2) << seconds;
				return os.str();
			})->SetColor({255, 255, 255, 255})->SetYScale(0.14f),
			gui::NewDynamicLabel("", [&]()->std::string {
				return "Score: " + std::to_string(playableLevel->GetScore());
			})->SetColor({255, 255, 255, 255})->SetYScale(0.14f),
			gui::NewLabel("Refresh the page to play again")->SetColor({255, 255, 255, 255})->SetYScale(0.14f)
		})->SetAlignment(gui::Alignment::Center)->SetColor({0, 0, 0, 210})
	);

	// Load shader and set up some uniforms
	Shader shader = LoadShader(TextFormat((assetPrefix + "assets/shaders/glsl%i/lighting.vs").c_str(), GLSL_VERSION),
		TextFormat((assetPrefix + "assets/shaders/glsl%i/fog.fs").c_str(), GLSL_VERSION));
	shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
	shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	int ambientLoc = GetShaderLocation(shader, "ambient");
	float ambientColor[4] = {0.7f, 0.7f, 0.7f, 1.0f};
	SetShaderValue(shader, ambientLoc, ambientColor, SHADER_UNIFORM_VEC4);
	float fogDensity = 0.00f;
	SetShaderValue(shader, GetShaderLocation(shader, "fogDensity"), &fogDensity, SHADER_UNIFORM_FLOAT);
	model::manager::SetShaders(shader);
	CreateLight(LIGHT_POINT, Vector3{0, 500, 0}, Vector3{0, 0, 0}, WHITE, shader);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}
#endif

	gui::fontmanager::Unload();
	model::manager::Unload();
	CloseWindow();
	return 0;
}

void UpdateDrawFrame(void) {
	if ((IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) && !playableLevel->IsGameOver()) {
		if (playableLevel->IsPaused()) {
			playableLevel->Unpause();
		} else {
			playableLevel->Pause();
		}
	}
	stats::Frame::StartFrame();
	float deltaTime = GetFrameTime();
	mouseInfo.Update();
	menu->Update(mouseInfo, screenRect);

	//if (IsKeyPressed(KEY_I)) {
	//	level->SpawnEnemy(character::EnemyType::Skull, 1);
	//}
	//if (IsKeyPressed(KEY_O)) {
	//	level->SpawnEnemy(character::EnemyType::Turret, 1);
	//}
	//if (IsKeyPressed(KEY_P)) {
	//	level->SpawnEnemy(character::EnemyType::Vampire, 1);
	//}

	// switching weapons
	if (IsKeyPressed(KEY_ONE)) {
		playableLevel->GetPlayer()->SetCurrentWeapon(character::Player::WeaponType::PISTOL);
	}
	if (IsKeyPressed(KEY_TWO)) {
		playableLevel->GetPlayer()->SetCurrentWeapon(character::Player::WeaponType::SHOTGUN);
	}
	if (IsKeyPressed(KEY_THREE)) {
		playableLevel->GetPlayer()->SetCurrentWeapon(character::Player::WeaponType::SNIPER);
	}

	BeginDrawing();
	ClearBackground({255, 255, 255, 255});
	BeginMode3D(*playableLevel->GetPlayer());

	// Update level
	playableLevel->Update(mouseInfo, deltaTime);
	playableLevel->Draw(deltaTime);
	EndMode3D();
	stats::Frame::EndFrame();
	if(!playableLevel->IsGameOver()) {
		if (!playableLevel->IsPaused()) {
			menu->Draw(screenRect.PosX, screenRect.PosY, screenRect.ContainerWidth, screenRect.ContainerHeight);
			centerDot->Draw(screenRect.PosX, screenRect.PosY, screenRect.ContainerWidth, screenRect.ContainerHeight);
		} else {
			paused->Draw(screenRect.PosX, screenRect.PosY, screenRect.ContainerWidth, screenRect.ContainerHeight);
		}
	} else {
		gameOverScreen->Draw(screenRect.PosX, screenRect.PosY, screenRect.ContainerWidth, screenRect.ContainerHeight);
	}
	EndDrawing();
}
