// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/turret.h>
#include <random.h>
#include <render/model.h>

character::Turret::Turret(glm::vec3 pos) : Enemy(new physics::CapsuleBody(pos, {0, 1.005f, 0}, {0, 1.0f, 0}, 1.0f)) {
	model = model::manager::Get(model::manager::Name::Turret);
	body->SetLookAngleOffsets({PI / 2.0f, 0});
}

void character::Turret::Draw(float deltaTime) {
	glm::vec3 pos = body->GetPosition();
	render::Model(model, body);
}

glm::vec3 character::Turret::FindTarget(glm::vec3 playerPos) {
	// Determine how to move on XZ-plane
	float x = random::GetRandomRange(radMin, radMax);
	if (random::GetRandomRange(0.f, 1.f) >= 0.5) {
		x = -x;
	}
	float z = random::GetRandomRange(radMin, radMax);
	if (random::GetRandomRange(0.f, 1.f) >= 0.5) {
		z = -z;
	}

	glm::vec3 pos = body->GetPosition();
	glm::vec3 fuzz = {x, 0.0f, z};
	return playerPos + fuzz;
}

void character::Turret::Update(glm::vec3 playerPos, float deltaTime) {
	// After some amount of time, reset the target
	moveTime -= deltaTime;
	auto bodyPos = body->GetPosition();
	if (moveTime <= 0.0f) {
		moveTime = TURRET_MOVETIME;
		glm::vec3 target = FindTarget(playerPos);
		glm::vec3 delta = target - bodyPos;
		delta.y = target.y + 3.0f;
		body->SetPosition(bodyPos + delta);
	}
	playerPos.y = bodyPos.y;
	body->LookAt(playerPos);
	body->Update(deltaTime);
}
