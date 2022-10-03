// Copyright © 2022 Daylon Wilkins & James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <vector>

#include <raylib.h>
#include <gui/gui.h>
#include <level.h>

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

	gui::fontmanager::Load();
	auto mouse = mouse::Info{};
	std::shared_ptr<character::Player> player = std::make_shared<character::Player>(4.0f, 2.0f, 4.0f);
	Ray ray = {0};
	RayCollision collision = {0};

	auto menu = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({})
	);

	// TODO: move into level 1 constructor
	// initialize level
	level::Level level = level::Level();
	//level.SetPlayerSpawn({0, 0, 0});
	std::vector<glm::vec3> enemySpawns;
	enemySpawns.push_back(glm::vec3(0, 0, 0));
	level.SetEnemySpawns(enemySpawns);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
		mouse.Update();
		player.UpdatePosition(mouse);
		menu->Update(mouse, screenRect);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (!collision.hit) {
				ray = GetMouseRay({screenRect.ContainerWidth / 2.0f, screenRect.ContainerHeight / 2.0f}, player);
			} else {
				collision.hit = false;
			}
		}

		if (IsKeyPressed(KEY_I)) {
			level.SpawnEnemy(level::Level::EnemyType::SKULL, 1);
		}
		if (IsKeyPressed(KEY_O)) {
			level.SpawnEnemy(level::Level::EnemyType::TURRET, 1);
		}
		if (IsKeyPressed(KEY_P)) {
			level.SpawnEnemy(level::Level::EnemyType::VAMPIRE, 1);
		}

		BeginDrawing();
		ClearBackground({255, 255, 255, 255});
		BeginMode3D(player);
		DrawRay(ray, BLUE);
		DrawGrid(10, 1.0f);

		// Update level
		level.SetPlayerPos(player.GetPosition()); // TODO: something else
		level.Update();
		level.Draw();

		EndMode3D();
		menu->Draw(screenRect.PosX, screenRect.PosY, screenRect.ContainerWidth, screenRect.ContainerHeight);
		EndDrawing();
	}
#endif

	gui::fontmanager::Unload();
	CloseWindow();
	return 0;
}

void UpdateDrawFrame(void) {
	BeginDrawing();
	ClearBackground({255, 255, 255, 255});
	EndDrawing();
}
