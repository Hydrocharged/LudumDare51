// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/skull.h>
#include <random.h>
#include <render/model.h>

character::Skull::Skull(glm::vec3 pos) : Enemy(new physics::CapsuleBody(pos, {0, 0.505f, 0}, {0, 0.5f, 0}, 0.9f)) {
	model = model::manager::Get(model::manager::Name::Skull);
	body->SetLookAngleOffsets({PI / 2.0f, 0});
	body->SetGravity(false);
};

void character::Skull::Draw(float deltaTime) {
	glm::vec3 pos = body->GetPosition();
	render::Model(model, body, glm::vec3(1.0f));
}

void character::Skull::SetTarget(glm::vec3 playerPos) {
	// Determine how to move on XZ-plane
	float x = random::GetRandomRange(radMin, radMax);
	if (random::GetRandomRange(0.f, 1.f) >= 0.5) {
		x = -x;
	}
	float z = random::GetRandomRange(radMin, radMax);
	if (random::GetRandomRange(0.f, 1.f) >= 0.5) {
		z = -z;
	}

	// Determine how to move on Y-axis, must be above player to go down
	glm::vec3 pos = body->GetPosition();
	float y = random::GetRandomRange(0.f, radMin);
	if (random::GetRandomRange(0.f, 1.f) >= 0.75 && pos.y > playerPos.y) {
		y = -y;
	}

	glm::vec3 fuzz = {x, y, z};
	target = playerPos + fuzz;
	body->LookAt(target);
}

void character::Skull::Update(glm::vec3 playerPos, float deltaTime) {
	//TODO: figure out how to make it look at player later

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
}
