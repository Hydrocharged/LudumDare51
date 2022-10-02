// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.


#include <character/vampire.h>

void character::Vampire::Draw() {
	Color c = WHITE;
	if (moveState == 3) {
		c = YELLOW;
	}
	glm::vec3 pos = model[3];
	DrawModel(*modelObj, (Vector3){pos.x, pos.y, pos.z}, 1.0f, c);
}

void character::Vampire::SetTarget(glm::vec3 playerPos) {
	glm::vec3 pos = model[3];
	glm::vec3 dir = glm::normalize(playerPos - pos);
	target = playerPos + 2.f * dir;
	target.y = 0.0f;
}

void character::Vampire::Update(glm::vec3 playerPos) {
	glm::vec3 pos = model[3];

	// Adjust speed according to state
	// FSM might be a small brain solution, but it definitely works
	float dist = 0.f;
	switch (moveState) {
		case 0:
			speed = 0;
			stopTime -= GetFrameTime();
			if (stopTime <= 0.f) {
				moveState++;
				stopTime = VAMPIRE_STOPTIME;
			}
			break;
		case 1:
			speed = VAMPIRE_WALK_SPEED;
			moveTime -= GetFrameTime();
			SetTarget(playerPos);
			if (moveTime <= 0.f) {
				moveState++;
				moveTime = VAMPIRE_MOVETIME;
			}
			break;
		case 2:
			speed = 0;
			stopTime -= GetFrameTime();
			if (stopTime <= 0.f) {
				moveState++;
				stopTime = VAMPIRE_STOPTIME;
				SetTarget(playerPos);
			}
			break;
		case 3:
			speed = VAMPIRE_DASH_SPEED;
			dist = glm::distance(target, pos);
			if (dist <= 0.1f) {
				moveState++;
			}
			break;
		default:
			moveState = 0;
			speed = 0;
	}

	// go towards target
	glm::vec3 dir = glm::normalize(target - pos);
	model = glm::translate(model, GetFrameTime() * speed * dir);

}