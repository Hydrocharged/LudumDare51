// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.


#include <character/turrent.h>

void character::Turret::Draw() {
	glm::vec3 pos = model[3];
	DrawCube({pos.x, pos.y, pos.z}, 1, 2, 1, BLUE);
}

glm::vec3 FindTarget(glm::vec3 playerPos) {

}

void character::Turret::Update(glm::vec3 playerPos) {
	// After some amount of time, reset the target
	moveTime -= GetFrameTime();
	if (moveTime <= 0.0f) {
		moveTime = TURRENT_MOVETIME;
		glm::vec3 pos = model[3];
		glm::translate(playerPos - pos);
	}

}