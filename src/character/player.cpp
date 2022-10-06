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
#include <render/model.h>
#include <glm/gtx/quaternion.hpp>
#include <sound/raylib.h>

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
	glm::mat4 bodyRotMatrix = body->GetRotationMatrix();

	glm::vec3 weaponOffset = glm::normalize(glm::vec3(-glm::normalize(glm::vec4{-0.26f, 0.26f, 0.35f, 0.0f}) * bodyRotMatrix));
	glm::vec3 weaponPos = GetCameraPosition() + (weaponOffset * 2.0f);

	glm::vec3 rotAxis = glm::vec3(-glm::normalize(glm::vec4{0.21f, 6.21005f, 0.13f, 0}) * bodyRotMatrix);
	glm::mat4 rotMatrix = glm::rotate(bodyRotMatrix, -4.76001f, rotAxis);

	switch (currentWeapon) {
		case PISTOL:
			render::Model(pistol, weaponPos, rotMatrix, glm::vec3{0.8f});
			break;
		case SHOTGUN:
			render::Model(shotgun, weaponPos, rotMatrix);
			break;
		case SNIPER:
			render::Model(sniper, weaponPos, rotMatrix);
			break;
	}
}

void character::Player::UpdatePosition(mouse::Info& mouse, float deltaTime) {
	dashReserves += deltaTime;
	if (dashReserves > 1.2f) {
		dashReserves = 1.2f;
	}
	invincibilityTimer -= deltaTime;
	if (invincibilityTimer < 0.0f) {
		invincibilityTimer = 0.0f;
	}
	if (IsKeyPressed(KEY_UP)) {
		mouseSensitivity += 0.04f;
	} else if (IsKeyPressed(KEY_DOWN)) {
		mouseSensitivity -= 0.04f;
		if (mouseSensitivity <= 0) {
			mouseSensitivity = 0.04f;
		}
	}

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
		if (dashReserves >= 0.6f && (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_F) || IsKeyPressed(KEY_R) || IsKeyPressed(KEY_LEFT_CONTROL))) {
			dashReserves -= 0.6f;
			invincibilityTimer += 0.25;
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
		if (glm::abs(body->GetVelocity().y) < 0.1f) {
			body->ApplyInstantForce({0, 1.0f, 0}, jumpForce);
			jumpReserves = 1;
		} else if (jumpReserves > 0) {
			body->ApplyInstantForce({0, 1.0f, 0}, jumpForce);
			jumpReserves -= 1;
		}
	}
	body->Update(deltaTime);

	angleX -= (mouse.CurrentX - mouse.PrevX) * mouseSensitivity * deltaTime;
	angleY -= (mouse.CurrentY - mouse.PrevY) * mouseSensitivity * deltaTime;

	physics::SanitizeAngles(angleX, angleY);
	auto targetVec = physics::GetPositionTargetVector(body->GetPosition() + cameraHeight, angleX, angleY);
	camera->target.x = targetVec.x;
	camera->target.y = targetVec.y;
	camera->target.z = targetVec.z;
	body->SetLookAngles({angleX, angleY});

	if (pistolCooldown >= 0) {
		pistolCooldown -= deltaTime;
		if (pistolCooldown < 0) {
			pistolCooldown = 0.0f;
		}
	}
	if (shotgunCooldown >= 0) {
		shotgunCooldown -= deltaTime;
		if (shotgunCooldown < 0) {
			shotgunCooldown = 0.0f;
		}
	}
	if (sniperCooldown >= 0) {
		sniperCooldown -= deltaTime;
		if (sniperCooldown < 0) {
			sniperCooldown = 0.0f;
		}
	}
}

void character::Player::SetCurrentWeapon(character::Player::WeaponType weapon) {
	currentWeapon = weapon;
}

float character::Player::GetWeaponCooldownPercentage() {
	switch (currentWeapon) {
		case PISTOL:
			return pistolCooldown / PISTOL_FIRE_RATE;
		case SHOTGUN:
			return shotgunCooldown / SHOTGUN_FIRE_RATE;
		case SNIPER:
			return sniperCooldown / SNIPER_FIRE_RATE;
		default:
			return 0.0f;
	}
}

bool character::Player::CanShoot() {
	// don't do anything if gun is on "cooldown"
	switch (currentWeapon) {
		case PISTOL:
			return (pistolCooldown <= 0.0f);
		case SHOTGUN:
			return (shotgunCooldown <= 0.0f);
		case SNIPER:
			return (sniperCooldown <= 0.0f);
		default:
			return false;
	}
}

std::vector<character::Projectile*> character::Player::Shoot() {
	Ray r = GetMouseRay({(float)GetRenderWidth() / 2.f, (float)GetRenderHeight() / 2.f}, *camera.get());
	glm::vec3 dir = {r.direction.x, r.direction.y, r.direction.z};
	glm::vec3 offset = 0.25f * dir;

	glm::mat4 bodyRotMatrix = body->GetRotationMatrix();
	glm::vec3 rotAxis = glm::vec3(-glm::normalize(glm::vec4{0.21f, 6.21005f, 0.13f, 0}) * bodyRotMatrix);
	glm::mat4 rotMatrix = glm::rotate(body->GetRotationMatrix(), -4.76001f, rotAxis);

	std::vector<character::Projectile*> projectiles;
	bool useBoostedDamage = false;
	if (ammo > 0) {
		useBoostedDamage = true;
	}
	switch (currentWeapon) {
		case PISTOL: {
			PlaySound(sound::manager::Get(sound::manager::Name::Pistol));
			ammo -= PISTOL_AMMO;
			if (ammo < 0) {
				health += ammo;
				ammo = 0;
			}
			if (health < 0) {
				health = 0;
			}

			float damage = 35.0f;
			if (useBoostedDamage) {
				damage *= ((1.0f - health / maxHealth) * 4.0f) + 1.0f;
			}
			pistolCooldown = PISTOL_FIRE_RATE;
			projectiles.push_back(new character::PistolProjectile(damage, this->GetCameraPosition() + offset, dir, rotMatrix));
			break;
		}
		case SHOTGUN: {
			PlaySound(sound::manager::Get(sound::manager::Name::Shotgun));
			ammo -= SHOTGUN_AMMO;
			if (ammo < 0) {
				health += ammo;
				ammo = 0;
			}
			if (health < 0) {
				health = 0;
			}

			float damage = 20.0f;
			if (useBoostedDamage) {
				damage *= ((1.0f - health / maxHealth) * 4.0f) + 1.0f;
			}
			shotgunCooldown = SHOTGUN_FIRE_RATE;
			for (int i = 0; i < 10; i++) {
				glm::vec3 jitter = {rando::GetRandomRange(-0.25f, 0.25f), rando::GetRandomRange(-0.25f, 0.25f), rando::GetRandomRange(-0.25f, 0.25f)};
				projectiles.push_back(new character::ShotgunProjectile(damage, this->GetCameraPosition() + offset, dir + jitter, rotMatrix));
			}
			break;
		}
		case SNIPER:
			PlaySound(sound::manager::Get(sound::manager::Name::Sniper));
			ammo -= SNIPER_AMMO;
			if (ammo < 0) {
				health += ammo;
				ammo = 0;
			}
			if (health < 0) {
				health = 0;
			}

			float damage = 100.0f;
			if (useBoostedDamage) {
				damage *= ((1.0f - health / maxHealth) * 4.0f) + 1.0f;
			}
			sniperCooldown = SNIPER_FIRE_RATE;
			projectiles.push_back(new character::SniperProjectile(damage, this->GetCameraPosition() + offset, dir, rotMatrix));
			break;
	}

	return projectiles;
}

void character::Player::TakeDamage(float dmg) {
	if (invincibilityTimer > 0) {
		return;
	}
	health -= dmg;
	if (health < 0) {
		health = 0;
	} else {
		PlaySound(sound::manager::Get(sound::manager::Name::Ouch));
	}
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
