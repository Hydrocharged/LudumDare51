// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/turret.h>
#include <random.h>
#include <render/model.h>

character::Turret::Turret(glm::vec3 pos) : Enemy(new physics::CapsuleBody(pos,
	{0, 1.005f, 0}, {0, 1.0f, 0}, 1.0f), model::manager::Get(model::manager::Name::Turret)) {
	body->SetLookAngleOffsets({PI / 2.0f, 0});
	cooldown = TURRET_FIRE_RATE;
}

void character::Turret::Draw(float deltaTime) {
	glm::vec3 pos = body->GetPosition();
	TintModel();
	render::Model(model, body);
	UntintModel();
}

void character::Turret::SetTarget(glm::vec3 playerPos) {
	// Determine how to move on XZ-plane
	float x = rando::GetRandomRange(radMin, radMax);
	if (rando::GetRandomRange(0.f, 1.f) >= 0.5) {
		x = -x;
	}
	float z = rando::GetRandomRange(radMin, radMax);
	if (rando::GetRandomRange(0.f, 1.f) >= 0.5) {
		z = -z;
	}

	glm::vec3 pos = body->GetPosition();
	glm::vec3 fuzz = {x, 0.0f, z};
	target = playerPos + fuzz;
}

void character::Turret::Update(glm::vec3 playerPos, float deltaTime) {
	// After some amount of time, reset the target
	moveTime -= deltaTime;
	if (cooldown >= 0) { cooldown -= deltaTime; }
	if (meleeCooldown >= 0.f) {
		meleeCooldown -= deltaTime;
	}
	playerLoc = playerPos;
	auto bodyPos = body->GetPosition();
	if (moveTime <= 0.0f) {
		moveTime = TURRET_MOVETIME;
		SetTarget(playerPos);
		glm::vec3 delta = target - bodyPos;
		delta.y = target.y + 3.0f;
		body->SetPosition(bodyPos + delta);
	}
	playerPos.y = bodyPos.y;
	body->LookAt(playerPos);
	body->Update(deltaTime);
}

character::Projectile* character::Turret::Shoot() {
	cooldown = TURRET_FIRE_RATE;
	glm::vec3 pos = body->GetPosition();
	glm::vec3 dir = glm::normalize(playerLoc - pos);
	return new character::Projectile(false, 5.0f, 0.2f, 10, 10.0f, body->GetPosition(), dir, body->GetRotationMatrix());
}
