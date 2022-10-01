// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <raylib.h>
#include <player/player.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif // PLATFORM_WEB

#ifdef _WIN32
#include <hideconsole.h>

#endif //_WIN32

// TODO: how to disable swinging and tilting
#define CAMERA_FIRST_PERSON_SWINGING_DELTA 0.0f
#define CAMERA_FIRST_PERSON_TILTING_DELTA 0.0f

void UpdateDrawFrame(void);

int main(void) {
#ifdef _WIN32
	HideConsole();
#endif //_WIN32

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "RaylibStarter");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	player::Player player(4.0f, 2.0f, 4.0f);

	Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
	Vector3 cubeSize = {2.0f, 2.0f, 2.0f};

	Ray ray = {0};
	RayCollision collision = {0};

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	while (!WindowShouldClose()) {
		UpdateCamera(player.GetCamera());

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (!collision.hit) {
				ray = GetMouseRay({640, 360}, *player.GetCamera());

				// Check collision between ray and box
				collision = GetRayCollisionBox(ray,
					(BoundingBox){(Vector3){cubePosition.x - cubeSize.x / 2, cubePosition.y - cubeSize.y / 2, cubePosition.z - cubeSize.z / 2},
								  (Vector3){cubePosition.x + cubeSize.x / 2, cubePosition.y + cubeSize.y / 2, cubePosition.z + cubeSize.z / 2}});
			} else { collision.hit = false; }
		}

		BeginDrawing();
		ClearBackground({0, 0, 0, 255});

		BeginMode3D(*player.GetCamera());
		if (collision.hit) {
			DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
			DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, MAROON);

			DrawCubeWires(cubePosition, cubeSize.x + 0.2f, cubeSize.y + 0.2f, cubeSize.z + 0.2f, GREEN);
		} else {
			DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, GRAY);
			DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, DARKGRAY);
		}

		DrawRay(ray, MAROON);
		DrawGrid(10, 1.0f);

		EndMode3D();

		DrawRectangle(10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
		DrawRectangleLines(10, 10, 320, 133, BLUE);

		DrawText("First person camera default controls:", 20, 20, 10, BLACK);
		DrawText("- Move with keys: W, A, S, D", 40, 40, 10, DARKGRAY);
		DrawText("- Mouse move to look around", 40, 60, 10, DARKGRAY);
		DrawFPS(10, 10);

		EndDrawing();
	}
#endif

	CloseWindow();
	return 0;
}

void UpdateDrawFrame(void) {
	BeginDrawing();
	ClearBackground({0, 0, 0, 255});
	EndDrawing();
}
