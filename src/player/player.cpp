// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <player/player.h>

player::Player::Player(float x, float y, float z) {
	camera = std::unique_ptr<Camera>(new Camera{0});
	camera->position = (Vector3){x, y, z};
	camera->target = (Vector3){0.0f, 1.8f, 0.0f};
	camera->up = (Vector3){0.0f, 1.0f, 0.0f};
	camera->fovy = 90.0f;
	camera->projection = CAMERA_PERSPECTIVE;

	SetCameraMode(*camera, CAMERA_FIRST_PERSON);
}

Camera* player::Player::GetCamera() {
	return camera.get();
}
