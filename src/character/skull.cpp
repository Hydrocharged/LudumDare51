// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/skull.h>

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
	glm::vec3 pos = model[3];
	float y = random::GetRandomRange(0.f, radMin);
	if (random::GetRandomRange(0.f, 1.f) >= 0.5 && pos.y > playerPos.y) {
		y = -y;
	}

	glm::vec3 fuzz = {x,y,z};
	target = playerPos + fuzz;
}

void character::Skull::Update(glm::vec3 playerPos) {
	// TODO: GetFrameTime should be replaced with some global variable

	// Move towards target
	glm::vec3 pos = model[3];
	glm::vec3 dir = glm::normalize(target - pos);
	model = glm::translate(model, GetFrameTime() * speed * dir);

	// After some amount of time, reset the target
	moveTime -= GetFrameTime();
	if (moveTime <= 0.0f) {
		moveTime = SKULL_MOVETIME;
		SetTarget(playerPos);
	}

	// TODO: figure out how to make it look at player later
	//glm::mat4 view = glm::lookAt(pos, playerPos, {0,1,0});
	//glm::vec3 forward = normalize(glm::vec3(glm::inverse(view)[2]));
}

void character::Skull::Draw() {
	glm::vec3 pos = model[3];
	DrawCube({pos.x, pos.y, pos.z}, 1, 1, 1, RED);
}