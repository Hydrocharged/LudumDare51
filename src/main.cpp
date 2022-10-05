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

int main(void) {
#ifdef _WIN32
	HideConsole();
#endif //_WIN32

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	auto screenRect = gui::DrawRect{0, 0, 1280, 720};
	InitWindow((int)screenRect.ContainerWidth, (int)screenRect.ContainerHeight, "10 Seconds Till Death");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	model::manager::Load();
	gui::fontmanager::Load();
	auto mouse = mouse::Info{};
	auto menu = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({
			gui::NewDynamicLabel("", []()->std::string {
				return stats::Frame::Current()->TotalFrametimeString();
			})->SetYScale(0.1f)->SetColor(RED)
		})->SetAlignment(gui::Alignment::Start)
	);
	const float centerDotSize = 0.008f;
	auto centerDot = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({
			gui::NewHorizontalPanel({})->SetColor({0, 0, 0, 255})->
				SetXScale((screenRect.ContainerHeight / screenRect.ContainerWidth) * centerDotSize)->SetYScale(centerDotSize)
		})->SetAlignment(gui::Alignment::Center)
	);

	auto level = level::GetLevel1();

	// Load shader and set up some uniforms
	Shader shader = LoadShader(TextFormat((assetPrefix + "assets/shaders/glsl%i/lighting.vs").c_str(), GLSL_VERSION),
		TextFormat((assetPrefix + "assets/shaders/glsl%i/fog.fs").c_str(), GLSL_VERSION));
	shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
	shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	int ambientLoc = GetShaderLocation(shader, "ambient");
	float ambientColor[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	SetShaderValue(shader, ambientLoc, ambientColor, SHADER_UNIFORM_VEC4);
	float fogDensity = 0.00f;
	SetShaderValue(shader, GetShaderLocation(shader, "fogDensity"), &fogDensity, SHADER_UNIFORM_FLOAT);
	model::manager::SetShaders(shader);
	CreateLight(LIGHT_POINT, Vector3{0, 500, 0}, Vector3{0, 0, 0}, WHITE, shader);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
		stats::Frame::StartFrame();
		float deltaTime = GetFrameTime();
		mouse.Update();
		menu->Update(mouse, screenRect);

		if (IsKeyPressed(KEY_I)) {
			level->SpawnEnemy(character::EnemyType::Skull, 1);
		}
		if (IsKeyPressed(KEY_O)) {
			level->SpawnEnemy(character::EnemyType::Turret, 1);
		}
		if (IsKeyPressed(KEY_P)) {
			level->SpawnEnemy(character::EnemyType::Vampire, 1);
		}

		// switching weapons
		if (IsKeyPressed(KEY_ONE)) {
			level->GetPlayer()->SetCurrentWeapon(character::Player::WeaponType::PISTOL);
		}
		if (IsKeyPressed(KEY_TWO)) {
			level->GetPlayer()->SetCurrentWeapon(character::Player::WeaponType::SHOTGUN);
		}
		if (IsKeyPressed(KEY_THREE)) {
			level->GetPlayer()->SetCurrentWeapon(character::Player::WeaponType::SNIPER);
		}

		BeginDrawing();
		ClearBackground({255, 255, 255, 255});
		BeginMode3D(*level->GetPlayer());

		// Update level
		level->Update(mouse, deltaTime);
		level->Draw(deltaTime);
		EndMode3D();
		stats::Frame::EndFrame();
		menu->Draw(screenRect.PosX, screenRect.PosY, screenRect.ContainerWidth, screenRect.ContainerHeight);
		//TODO: only draw the dot if the level is running
		centerDot->Draw(screenRect.PosX, screenRect.PosY, screenRect.ContainerWidth, screenRect.ContainerHeight);
		EndDrawing();
	}
#endif

	gui::fontmanager::Unload();
	model::manager::Unload();
	CloseWindow();
	return 0;
}

void UpdateDrawFrame(void) {
	BeginDrawing();
	ClearBackground({255, 255, 255, 255});
	EndDrawing();
}
