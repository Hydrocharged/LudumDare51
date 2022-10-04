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

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif // PLATFORM_WEB

#ifdef _WIN32
#include <hideconsole.h>

#endif //_WIN32

void UpdateDrawFrame(void);

int main(void) {
#ifdef _WIN32
	HideConsole();
#endif //_WIN32

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	auto screenRect = gui::DrawRect{0, 0, 1280, 720};
	InitWindow((int)screenRect.ContainerWidth, (int)screenRect.ContainerHeight, "RaylibStarter");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	model::manager::Load();
	gui::fontmanager::Load();
	auto mouse = mouse::Info{};
	auto menu = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({})
	);
	auto level = level::GetLevel1();

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
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
		if (IsKeyPressed(KEY_FOUR)) {
			level->GetPlayer()->SetCurrentWeapon(character::Player::WeaponType::MELEE);
		}

		BeginDrawing();
		ClearBackground({255, 255, 255, 255});
		BeginMode3D(*level->GetPlayer());

		// Update level
		level->Update(mouse, deltaTime);
		level->Draw(deltaTime);
		EndMode3D();
		menu->Draw(screenRect.PosX, screenRect.PosY, screenRect.ContainerWidth, screenRect.ContainerHeight);
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
