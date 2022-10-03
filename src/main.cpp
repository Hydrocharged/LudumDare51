// Copyright © 2022 Daylon Wilkins & James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <vector>

#include <raylib.h>
#include <gui/gui.h>
#include <level.h>
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

	gui::fontmanager::Load();
	auto mouse = mouse::Info{};
	auto menu = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({})
	);
	
	// initialize level
	level::Level level = level::Level();
	level.SetDimensions({64, 64, 64});
	level.SetPlayerSpawn({0, 2, 0});
	level.SpawnPlayer();
	std::vector<glm::vec3> enemySpawns;
	enemySpawns.push_back(glm::vec3(0, 0, 0));
	level.SetEnemySpawns(enemySpawns);

	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{0, -0.5, 0}, glm::vec3{32, 1, 32}));

	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{0, 0.5, 20}, glm::vec3{32, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{0, 0.5, -20}, glm::vec3{32, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{20, 0.5, 0}, glm::vec3{8, 1, 32}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{-20, 0.5, 0}, glm::vec3{8, 1, 32}));

	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{0, 1.5, 28}, glm::vec3{48, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{0, 1.5, -28}, glm::vec3{48, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{28, 1.5, 0}, glm::vec3{8, 1, 48}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{-28, 1.5, 0}, glm::vec3{8, 1, 48}));

	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{20, 1.5, 20}, glm::vec3{8, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{20, 1.5, -20}, glm::vec3{8, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{-20, 1.5, 20}, glm::vec3{8, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{-20, 1.5, -20}, glm::vec3{8, 1, 8}));

	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{28, 2.5, 28}, glm::vec3{8, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{28, 2.5, -28}, glm::vec3{8, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{-28, 2.5, 28}, glm::vec3{8, 1, 8}));
	level.AddBody(std::make_shared<physics::AABBBody>(glm::vec3{-28, 2.5, -28}, glm::vec3{8, 1, 8}));

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
		mouse.Update();
		level.GetPlayer()->UpdatePosition(mouse);
		menu->Update(mouse, screenRect);

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
		BeginMode3D(*level.GetPlayer());

		// Update level
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
