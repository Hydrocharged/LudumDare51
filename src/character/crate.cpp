// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/crate.h>

character::Crate::Crate(float health, float ammo, glm::vec3 pos) {
	this->health = health;
	this->ammo = ammo;

	model = model::manager::Get(model::manager::Name::Crate);

	// Set up physics body
	body = std::make_unique<physics::AABBBody>(pos, glm::vec3{1, 2, 1});
	body->SetGravity(false);
	body->SetHorizontalDrag(0);
	body->SetVerticalDrag(0);
}

void character::Crate::Update(float deltaTime) {
	// TODO: spin
	body->Update(deltaTime);
}

void character::Crate::Draw(float deltaTime) {
	glm::vec3 pos = body->GetPosition();
	DrawModel(model, (Vector3){pos.x, pos.y, pos.z}, 0.4f, WHITE);
}


