// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <raylib.h>
#include <character/player.h>
#include <memory>
#include <cmath>

//TODO: speed should be physics-based, also add jumping and dodging mechanics

const float moveSpeedConstant = 40.0f;
const float sprintModifier = 3.0f;
const float dashModifier = 1.2f;
const float jumpForce = 15.0f;
const float mouseSensitivity = 0.5f;
const glm::vec3 cameraHeight = {0, 1.84f, 0};

character::Player::Player(glm::vec3 position) {
	Model pistol = LoadModel("../assets/models/pistol/pistol.obj");
	Texture2D pistolTexture = LoadTexture("../assets/models/pistol/pistol.png");
	SetMaterialTexture(&pistol.materials[0], MATERIAL_MAP_DIFFUSE, pistolTexture);
	this->pistol = std::make_unique<Model>(pistol);

	Model shotgun = LoadModel("../assets/models/shotgun/shotgun.obj");
	Texture2D shotgunTexture = LoadTexture("../assets/models/shotgun/shotgun.png");
	SetMaterialTexture(&shotgun.materials[0], MATERIAL_MAP_DIFFUSE, shotgunTexture);
	this->shotgun = std::make_unique<Model>(shotgun);

	Model sniper = LoadModel("../assets/models/sniper/sniper.obj");
	Texture2D sniperTexture = LoadTexture("../assets/models/sniper/sniper.png");
	SetMaterialTexture(&sniper.materials[0], MATERIAL_MAP_DIFFUSE, sniperTexture);
	this->sniper = std::make_unique<Model>(sniper);

	body = std::make_unique<physics::CapsuleBody>(position, cameraHeight, glm::vec3(0), 0.5f);

	camera = std::make_unique<Camera>(Camera{0});
	auto cameraPos = position + cameraHeight;
	camera->position = Vector3{cameraPos.x, cameraPos.y, cameraPos.z};
	camera->target = Vector3{0.0f, 0.0f, 0.0f};
	camera->up = Vector3{0.0f, 1.0f, 0.0f};
	camera->fovy = 90.0f;
	camera->projection = CAMERA_PERSPECTIVE;

	float dx = camera->target.x - camera->position.x;
	float dy = camera->target.y - camera->position.y;
	float dz = camera->target.z - camera->position.z;

	angleX = atan2f(dx, dz);
	angleY = atan2f(dy, sqrtf(dx * dx + dz * dz));
	DisableCursor();
}

character::Player::operator Camera() {
	glm::vec3 cameraPosition = body->Position() + cameraHeight;
	camera->position = {cameraPosition.x, cameraPosition.y, cameraPosition.z};
	return *camera.get();
}

character::Player::operator Camera*() {
	glm::vec3 cameraPosition = body->Position() + cameraHeight;
	camera->position = {cameraPosition.x, cameraPosition.y, cameraPosition.z};
	return camera.get();
}

void character::Player::Draw() {
	Vector3 pos = camera->position;
	pos.x += 1.0f;
	pos.y += -0.5f;
	pos.z += 1.0f;
	switch (currentWeapon) {
		case PISTOL:
			DrawModel(*pistol.get(), pos, 0.5f, WHITE);
			break;
		case SHOTGUN:
			DrawModel(*shotgun.get(), pos, 1.0f, WHITE);
			break;
		case SNIPER:
			DrawModel(*sniper.get(), pos, 1.0f, WHITE);
			break;
		case MELEE:
			break;
	}
}

void character::Player::UpdatePosition(mouse::Info& mouse) {
	auto forwardPressed = (float)IsKeyDown(KEY_W);
	auto backPressed = (float)IsKeyDown(KEY_S);
	auto leftPressed = (float)IsKeyDown(KEY_A);
	auto rightPressed = (float)IsKeyDown(KEY_D);

	glm::vec3 direction = {};
	direction.x += (sinf(angleX) * backPressed -
					sinf(angleX) * forwardPressed -
					cosf(angleX) * leftPressed +
					cosf(angleX) * rightPressed);
	direction.z += (cosf(angleX) * backPressed -
					cosf(angleX) * forwardPressed +
					sinf(angleX) * leftPressed -
					sinf(angleX) * rightPressed);
	if (glm::length(direction) > FLT_EPSILON) {
		if (IsKeyPressed(KEY_F)) {
			body->ApplyInstantForce(direction, dashModifier * moveSpeedConstant);
		} else {
			body->ApplyFrameForce(direction, moveSpeedConstant * (IsKeyDown(KEY_LEFT_SHIFT) ? sprintModifier : 1.0f));
		}
	} else {
		glm::vec3 hVelocity = body->GetHorizontalVelocity();
		float hMagnitude = glm::length(hVelocity);
		if (hMagnitude > FLT_EPSILON) {
			if (hMagnitude > 0.25f) {
				body->ApplyFrameForce(-glm::normalize(hVelocity), 20.0f);
			} else {
				body->ApplyInstantForce(-glm::normalize(hVelocity), hMagnitude);
			}
		}
	}
	if (IsKeyPressed(KEY_SPACE)) {
		body->ApplyInstantForce({0, 1.0f, 0}, jumpForce);
	}
	body->Update(GetFrameTime());

	angleX -= (mouse.CurrentX - mouse.PrevX) * mouseSensitivity * GetFrameTime();
	angleY -= (mouse.CurrentY - mouse.PrevY) * mouseSensitivity * GetFrameTime();

	if (angleY > 89.0f * DEG2RAD) { angleY = 89.0f * DEG2RAD; }
	else if (angleY < -89.0f * DEG2RAD) { angleY = -89.0f * DEG2RAD; }

	Matrix matTranslation = {1.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 1.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 1.0f, 10.0f,
							 0.0f, 0.0f, 0.0f, 1.0f};

	Matrix matRotation = {1.0f, 0.0f, 0.0f, 0.0f,
						  0.0f, 1.0f, 0.0f, 0.0f,
						  0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f};

	float cosz = cosf(0.0f);
	float sinz = sinf(0.0f);
	float cosy = cosf(-(PI * 2 - angleX));
	float siny = sinf(-(PI * 2 - angleX));
	float cosx = cosf(-(PI * 2 - angleY));
	float sinx = sinf(-(PI * 2 - angleY));

	matRotation.m0 = cosz * cosy;
	matRotation.m4 = (cosz * siny * sinx) - (sinz * cosx);
	matRotation.m8 = (cosz * siny * cosx) + (sinz * sinx);
	matRotation.m1 = sinz * cosy;
	matRotation.m5 = (sinz * siny * sinx) + (cosz * cosx);
	matRotation.m9 = (sinz * siny * cosx) - (cosz * sinx);
	matRotation.m2 = -siny;
	matRotation.m6 = cosy * sinx;
	matRotation.m10 = cosy * cosx;

	Matrix matTransform = {0};
	matTransform.m0 = matTranslation.m0 * matRotation.m0 + matTranslation.m1 * matRotation.m4 + matTranslation.m2 * matRotation.m8 + matTranslation.m3 * matRotation.m12;
	matTransform.m1 = matTranslation.m0 * matRotation.m1 + matTranslation.m1 * matRotation.m5 + matTranslation.m2 * matRotation.m9 + matTranslation.m3 * matRotation.m13;
	matTransform.m2 = matTranslation.m0 * matRotation.m2 + matTranslation.m1 * matRotation.m6 + matTranslation.m2 * matRotation.m10 + matTranslation.m3 * matRotation.m14;
	matTransform.m3 = matTranslation.m0 * matRotation.m3 + matTranslation.m1 * matRotation.m7 + matTranslation.m2 * matRotation.m11 + matTranslation.m3 * matRotation.m15;
	matTransform.m4 = matTranslation.m4 * matRotation.m0 + matTranslation.m5 * matRotation.m4 + matTranslation.m6 * matRotation.m8 + matTranslation.m7 * matRotation.m12;
	matTransform.m5 = matTranslation.m4 * matRotation.m1 + matTranslation.m5 * matRotation.m5 + matTranslation.m6 * matRotation.m9 + matTranslation.m7 * matRotation.m13;
	matTransform.m6 = matTranslation.m4 * matRotation.m2 + matTranslation.m5 * matRotation.m6 + matTranslation.m6 * matRotation.m10 + matTranslation.m7 * matRotation.m14;
	matTransform.m7 = matTranslation.m4 * matRotation.m3 + matTranslation.m5 * matRotation.m7 + matTranslation.m6 * matRotation.m11 + matTranslation.m7 * matRotation.m15;
	matTransform.m8 = matTranslation.m8 * matRotation.m0 + matTranslation.m9 * matRotation.m4 + matTranslation.m10 * matRotation.m8 + matTranslation.m11 * matRotation.m12;
	matTransform.m9 = matTranslation.m8 * matRotation.m1 + matTranslation.m9 * matRotation.m5 + matTranslation.m10 * matRotation.m9 + matTranslation.m11 * matRotation.m13;
	matTransform.m10 = matTranslation.m8 * matRotation.m2 + matTranslation.m9 * matRotation.m6 + matTranslation.m10 * matRotation.m10 + matTranslation.m11 * matRotation.m14;
	matTransform.m11 = matTranslation.m8 * matRotation.m3 + matTranslation.m9 * matRotation.m7 + matTranslation.m10 * matRotation.m11 + matTranslation.m11 * matRotation.m15;
	matTransform.m12 = matTranslation.m12 * matRotation.m0 + matTranslation.m13 * matRotation.m4 + matTranslation.m14 * matRotation.m8 + matTranslation.m15 * matRotation.m12;
	matTransform.m13 = matTranslation.m12 * matRotation.m1 + matTranslation.m13 * matRotation.m5 + matTranslation.m14 * matRotation.m9 + matTranslation.m15 * matRotation.m13;
	matTransform.m14 = matTranslation.m12 * matRotation.m2 + matTranslation.m13 * matRotation.m6 + matTranslation.m14 * matRotation.m10 + matTranslation.m15 * matRotation.m14;
	matTransform.m15 = matTranslation.m12 * matRotation.m3 + matTranslation.m13 * matRotation.m7 + matTranslation.m14 * matRotation.m11 + matTranslation.m15 * matRotation.m15;

	camera->target.x = camera->position.x - matTransform.m12;
	camera->target.y = camera->position.y - matTransform.m13;
	camera->target.z = camera->position.z - matTransform.m14;
}

void character::Player::SetCurrentWeapon(character::Player::WeaponType weapon) {
	currentWeapon = weapon;
}

glm::vec3 character::Player::GetPosition() {
	return body->Position();
}

physics::CapsuleBody* character::Player::GetBody() {
	return body.get();
}
