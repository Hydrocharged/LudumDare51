// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <raylib.h>
#include <character/player.h>
#include <physics/target.h>
#include <model/raylib.h>
#include <memory>
#include <cmath>
#include <random.h>

const glm::vec3 cameraHeight = {0, 1.84f, 0};

character::Player::Player(glm::vec3 position) {
	body = std::make_unique<physics::CapsuleBody>(position, cameraHeight, glm::vec3(0), 0.5f);

	camera = std::make_unique<Camera>(Camera{0});
	auto cameraPos = position + cameraHeight;
	camera->position = Vector3{cameraPos.x, cameraPos.y, cameraPos.z};
	camera->target = Vector3{0.0f, 0.0f, 0.0f};
	camera->up = Vector3{0.0f, 1.0f, 0.0f};
	camera->fovy = 90.0f;
	camera->projection = CAMERA_PERSPECTIVE;

	glm::vec2 angles = body->GetLookAngles();
	physics::GetAngleXY(position + cameraHeight, glm::vec3(0), angles);
	body->SetLookAngles(angles);
	DisableCursor();

	pistol = model::manager::Get(model::manager::Name::Pistol);
	shotgun = model::manager::Get(model::manager::Name::Shotgun);
	sniper = model::manager::Get(model::manager::Name::Sniper);
}

character::Player::operator Camera() {
	glm::vec3 cameraPosition = body->GetPosition() + cameraHeight;
	camera->position = {cameraPosition.x, cameraPosition.y, cameraPosition.z};
	return *camera.get();
}

character::Player::operator Camera*() {
	glm::vec3 cameraPosition = body->GetPosition() + cameraHeight;
	camera->position = {cameraPosition.x, cameraPosition.y, cameraPosition.z};
	return camera.get();
}

void character::Player::Draw(float deltaTime) {
	Vector3 pos = camera->position;
	pos.x += 1.0f;
	pos.y += -0.5f;
	pos.z += 1.0f;
	switch (currentWeapon) {
		case PISTOL:
			DrawModel(pistol, pos, 0.5f, WHITE);
			break;
		case SHOTGUN:
			DrawModel(shotgun, pos, 1.0f, WHITE);
			break;
		case SNIPER:
			DrawModel(sniper, pos, 1.0f, WHITE);
			break;
		case MELEE:
			break;
	}
}

void character::Player::UpdatePosition(mouse::Info& mouse, float deltaTime) {
	auto forwardPressed = (float)IsKeyDown(KEY_W);
	auto backPressed = (float)IsKeyDown(KEY_S);
	auto leftPressed = (float)IsKeyDown(KEY_A);
	auto rightPressed = (float)IsKeyDown(KEY_D);
	float angleX = body->GetLookAngleX();
	float angleY = body->GetLookAngleY();

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
	body->Update(deltaTime);

	angleX -= (mouse.CurrentX - mouse.PrevX) * mouseSensitivity * deltaTime;
	angleY -= (mouse.CurrentY - mouse.PrevY) * mouseSensitivity * deltaTime;

	physics::SanitizeAngles(angleX, angleY);
	auto targetVec = physics::GetTargetVector(body->GetPosition() + cameraHeight, angleX, angleY);
	camera->target.x = targetVec.x;
	camera->target.y = targetVec.y;
	camera->target.z = targetVec.z;
	body->SetLookAngles({angleX, angleY});
}

void character::Player::SetCurrentWeapon(character::Player::WeaponType weapon) {
	currentWeapon = weapon;
}

std::vector<character::Projectile*> character::Player::Shoot() {
	Ray r = GetMouseRay({(float)GetRenderWidth() / 2.f, (float)GetRenderHeight() / 2.f}, *camera.get());
	glm::vec3 dir = {r.direction.x, r.direction.y, r.direction.z};
	glm::vec3 offset = 0.25f * dir;

	std::vector<character::Projectile*> projectiles;
	switch (currentWeapon) {
		case PISTOL:
			projectiles.push_back(new character::Projectile(true, 50.0f, 0.5f, 10, 2.0f, this->GetCameraPosition() + offset, dir));
			break;
		case SHOTGUN:
			for (int i = 0; i < 10; i++) {
				glm::vec3 jitter = {random::GetRandomRange(0.0f, 0.5f), random::GetRandomRange(0.0f, 0.1f), random::GetRandomRange(0.0f, 0.1f)};
				projectiles.push_back(new character::Projectile(true, 50.0f, 0.5f, 10, 2.0f, this->GetCameraPosition() + offset, dir + jitter));
			}
			break;
		case SNIPER:
			projectiles.push_back(new character::Projectile(true, 100.0f, 0.2f, 100, 3.0f, this->GetCameraPosition() + offset, dir));
			break;
		case MELEE:
			projectiles.push_back(new character::Projectile(true, 0.0f, 0.2f, 50, 0.1f, this->GetCameraPosition(), dir));
			break;
	}

	return projectiles;
}

glm::vec3 character::Player::GetPosition() {
	return body->GetPosition();
}

glm::vec3 character::Player::GetCameraPosition() {
	return body->GetPosition() + cameraHeight;
}

physics::CapsuleBody* character::Player::GetBody() {
	return body.get();
}
