// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/skull.h>
#include <random.h>
#include <render/model.h>

character::Skull::Skull(glm::vec3 pos) : Enemy(new physics::CapsuleBody(pos, {0, 0.505f, 0}, {0, 0.5f, 0}, 0.9f),
	model::manager::Get(model::manager::Name::Skull)) {
	body->SetLookAngleOffsets({PI / 2.0f, 0});
	body->SetGravity(false);
	cooldown = SKULL_FIRE_RATE;
};

void character::Skull::Draw(float deltaTime) {
	glm::vec3 pos = body->GetPosition();
	TintModel();
	render::Model(model, body);
	UntintModel();
}

void character::Skull::SetTarget(glm::vec3 playerPos) {
	// Determine how to move on XZ-plane
	float x = rando::GetRandomRange(radMin, radMax);
	if (rando::GetRandomRange(0.f, 1.f) >= 0.5) {
		x = -x;
	}
	float z = rando::GetRandomRange(radMin, radMax);
	if (rando::GetRandomRange(0.f, 1.f) >= 0.5) {
		z = -z;
	}

	// Determine how to move on Y-axis, must be above player to go down
	glm::vec3 pos = body->GetPosition();
	float y = rando::GetRandomRange(0.f, radMin);
	if (rando::GetRandomRange(0.f, 1.f) >= 0.75 && pos.y > playerPos.y) {
		y = -y;
	}

	glm::vec3 fuzz = {x, y, z};
	target = playerPos + fuzz;
	body->LookAt(target);
}

void character::Skull::Update(glm::vec3 playerPos, float deltaTime) {
	// Move towards target
	glm::vec3 dir = target - body->GetPosition();
	if (glm::length(dir) > FLT_EPSILON) {
		body->ApplyFrameForce(glm::normalize(dir), speed);
	}

	// After some amount of time, reset the target
	moveTime -= deltaTime;
	if (moveTime <= 0.0f) {
		moveTime = SKULL_MOVETIME;
		SetTarget(playerPos);
	}
	body->Update(deltaTime);

	if (cooldown >= 0) { cooldown -= deltaTime; }
	if (meleeCooldown >= 0.f) {
		meleeCooldown -= deltaTime;
	}
}

character::Projectile* character::Skull::Shoot() {
	cooldown = SKULL_FIRE_RATE;
	glm::vec3 pos = body->GetPosition();
	glm::vec3 dir = glm::normalize(target - pos);
	return new character::SkullProjectile(body->GetPosition(), dir, body->GetRotationMatrix());
}
