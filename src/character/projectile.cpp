// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/projectile.h>
#include <render/model.h>
#include <model/raylib.h>

character::Projectile::Projectile(bool fromPlayer, float speed, float size, float damage, float lifeSpan, glm::vec3 pos, glm::vec3 dir) {
	this->fromPlayer = fromPlayer;
	this->damage = damage;
	this->lifeSpan = lifeSpan;
	this->model = model::manager::Get(model::manager::Name::Crate);

	// Set up physics body
	body = new physics::SphereBody(pos, size);
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
	render::Model(model, body, glm::vec3(0.1f));
}
