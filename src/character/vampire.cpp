// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/vampire.h>
#include <render/model.h>

character::Vampire::Vampire(glm::vec3 pos) : Enemy(new physics::CapsuleBody(pos, {0, 1.3f, 0}, {0, 1.0f, 0}, 1.0f)) {
	model = model::manager::Get(model::manager::Name::Vampire);
	body->SetLookAngleOffsets({-PI / 2.0f, 0});
}

void character::Vampire::Draw(float deltaTime) {
	Color c = WHITE;
	if (moveState == 3) {
		c = YELLOW;
	}
	glm::vec3 pos = body->GetPosition();
	render::Model(model, body, glm::vec3(1.0f));
}

void character::Vampire::SetTarget(glm::vec3 playerPos) {
	glm::vec3 pos = body->GetPosition();
	glm::vec3 dir = glm::normalize(playerPos - pos);
	target = playerPos + 2.f * dir;
	target.y = 0.0f;
	body->LookAt(target);
}

void character::Vampire::Update(glm::vec3 playerPos, float deltaTime) {
	glm::vec3 pos = body->GetPosition();

	// Adjust speed according to state
	// FSM might be a small brain solution, but it definitely works
	float dist = 0.f;
	switch (moveState) {
		case 0:
			speed = 0;
			stopTime -= deltaTime;
			if (stopTime <= 0.f) {
				moveState++;
				stopTime = VAMPIRE_STOPTIME;
			}
			break;
		case 1:
			speed = VAMPIRE_WALK_SPEED;
			moveTime -= deltaTime;
			SetTarget(playerPos);
			if (moveTime <= 0.f) {
				moveState++;
				moveTime = VAMPIRE_MOVETIME;
			}
			break;
		case 2:
			speed = 0;
			stopTime -= deltaTime;
			if (stopTime <= 0.f) {
				moveState++;
				stopTime = VAMPIRE_STOPTIME;
				SetTarget(playerPos);
			}
			break;
		case 3:
			speed = VAMPIRE_DASH_SPEED;
			dist = glm::distance(target, pos);
			if (dist <= 3.0f) {
				moveState++;
			}
			break;
		default:
			moveState = 0;
			speed = 0;
	}

	// go towards target
	glm::vec3 dir = glm::normalize(target - pos);
	body->ApplyFrameForce(dir, speed);
	body->Update(deltaTime);
}
