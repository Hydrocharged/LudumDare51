// Copyright © 2022 Daylon Wilkins & James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <raylib.h>
#include <vector>
#include <character/player.h>
#include <gui/gui.h>
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
	character::Player player({4.0f, 2.0f, 4.0f});
	auto centerItem = std::make_unique<physics::AABBBody>(
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{2.0f, 2.0f, 2.0f});

	auto playerItem = std::make_unique<physics::SphereBody>(
		glm::vec3{0.0f, -1.0f, 2.0f}, 0.25f);

	//auto playerItem = std::make_unique<physics::CapsuleBody>(
	//	glm::vec3{0.0f, -1.0f, 2.0f},
	//	glm::vec3{-0.25f, 0.0f, 0.0f},
	//	glm::vec3{0.25f, 0.0f, 0.0f}, 0.25f);

	//auto playerItem = std::make_unique<physics::AABBBody>(
	//	glm::vec3{0.0f, -1.0f, 2.0f},
	//	glm::vec3{0.25f, 0.25f, 0.25f});

	playerItem->SetParent(player.GetBody());

	auto menu = std::unique_ptr<gui::Component>(
		gui::NewVerticalPanel({})
	);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
		mouse.Update();
		player.UpdatePosition(mouse);
		menu->Update(mouse, screenRect);

		BeginDrawing();
		ClearBackground({255, 255, 255, 255});
		BeginMode3D(player);

		render::Body(playerItem.get(), YELLOW);
		if (centerItem->CollidesWith(playerItem.get())) {
			render::Body(centerItem.get(), RED);
			render::BodyWireframe(centerItem.get(), DARKGRAY);
		} else {
			render::Body(centerItem.get(), GRAY);
			render::BodyWireframe(centerItem.get(), DARKGRAY);
		}
		DrawGrid(10, 1.0f);

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
