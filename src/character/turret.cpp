// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.


#include <character/turret.h>

void character::Turret::Draw() {
	glm::vec3 pos = model[3];
	DrawCube({pos.x, pos.y, pos.z}, 0.25, 2, 0.25, BLUE);
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

	glm::vec3 pos = model[3];
	glm::vec3 fuzz = {x, 2.f, z};
	return playerPos + fuzz;
}

void character::Turret::Update(glm::vec3 playerPos) {
	// After some amount of time, reset the target
	moveTime -= GetFrameTime();
	if (moveTime <= 0.0f) {
		moveTime = TURRENT_MOVETIME;
		glm::vec3 pos = model[3];
		glm::vec3 target = FindTarget(playerPos);
		model = glm::translate(target - pos);
	}
}