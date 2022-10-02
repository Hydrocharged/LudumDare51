// Copyright © 2022 Daylon Wilkins & James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <raylib.h>
#include <vector>
#include <character/player.h>
#include <character/enemy.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif // PLATFORM_WEB

#ifdef _WIN32
#include <hideconsole.h>

#endif //_WIN32

std::vector<character::Enemy*> enemies;

void UpdateDrawFrame(void);

int main(void) {
#ifdef _WIN32
	HideConsole();
#endif //_WIN32

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "RaylibStarter");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	auto mouse = mouse::Info{};
	character::Player player(4.0f, 2.0f, 4.0f);
	Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
	Vector3 cubeSize = {2.0f, 2.0f, 2.0f};
	Ray ray = {0};
	RayCollision collision = {0};

	// initialize enemies
	for (int i = 0; i < 5; i ++) {
		enemies.push_back(new character::Enemy({2.0f * i, 2.0f * i, 2.0f * i}));
	}

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
		mouse.Update();
		player.UpdatePosition(mouse);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (!collision.hit) {
				ray = GetMouseRay({(float)GetRenderWidth()/2.0f, (float)GetRenderHeight()/2.0f}, player);
				collision = GetRayCollisionBox(ray,
					BoundingBox{
						Vector3{cubePosition.x - cubeSize.x / 2, cubePosition.y - cubeSize.y / 2, cubePosition.z - cubeSize.z / 2},
						Vector3{cubePosition.x + cubeSize.x / 2, cubePosition.y + cubeSize.y / 2, cubePosition.z + cubeSize.z / 2},
					}
				);
			} else {
				collision.hit = false;
			}
		}

		BeginDrawing();
		ClearBackground({255, 255, 255, 255});

		// TODO: add some of this logic to enemies
		BeginMode3D(player);
		if (collision.hit) {
			DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
			DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, DARKGRAY);
		} else {
			DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, GRAY);
			DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, DARKGRAY);
		}
		DrawRay(ray, BLUE);
		DrawGrid(10, 1.0f);
		EndMode3D();


		// Update enemies
		for (character::Enemy* enemy : enemies) {
			enemy->UpdatePosition(player.GetPosition());
			enemy->Draw();
		}

		EndDrawing();
	}
#endif

	CloseWindow();
	return 0;
}

void UpdateDrawFrame(void) {
	BeginDrawing();
	ClearBackground({255, 255, 255, 255});
	EndDrawing();
}
