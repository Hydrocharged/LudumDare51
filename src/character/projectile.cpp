// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/projectile.h>

character::Projectile::Projectile(bool fromPlayer, float speed, float size, float damage, float lifeSpan, glm::vec3 pos, glm::vec3 dir) {
	this->fromPlayer = fromPlayer;
	this->damage = damage;
	this->lifeSpan = lifeSpan;

	// Set up physics body
	body = std::make_unique<physics::SphereBody>(pos, size);
	body->SetGravity(false);
	body->SetHorizontalDrag(0);
	body->SetVerticalDrag(0);
	body->ApplyInstantForce(dir, speed);
}

void character::Projectile::Update(float deltaTime) {
	body->Update(deltaTime);
	lifeSpan -= deltaTime;

}

void character::Projectile::Draw(float deltaTime) {
	glm::vec3 pos = body->GetPosition();
	//DrawSphere((Vector3){0, 5, 0}, 2.0f, RED); // TODO: change color
	DrawSphere((Vector3){pos.x, pos.y, pos.z}, 0.1f, RED); // TODO: change color
}
