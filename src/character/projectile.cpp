// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/projectile.h>
#include <render/model.h>
#include <model/raylib.h>

character::Projectile::Projectile(bool fromPlayer, float speed, float size, float damage, float lifeSpan, glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix) {
	this->fromPlayer = fromPlayer;
	this->damage = damage;
	this->lifeSpan = lifeSpan;
	if (fromPlayer) {
		this->model = model::manager::Get(model::manager::Name::Bullet);
	} else {
		this->model = model::manager::Get(model::manager::Name::EnemyProjectile);
	}
	this->rotMatrix = rotMatrix;

	// Set up physics body
	body = new physics::SphereBody(pos, size);
	body->SetGravity(false);
	body->SetHorizontalDrag(0);
	body->SetVerticalDrag(0);
	body->ApplyInstantForce(dir, speed);
	if (!fromPlayer) {
		body->OffsetPosition({0, 0.7f, 0});
	}
}

void character::Projectile::Update(float deltaTime) {
	body->Update(deltaTime);
	lifeSpan -= deltaTime;
}

void character::Projectile::Draw(float deltaTime) {
	if (fromPlayer) {
		render::Model(model, body, rotMatrix, glm::vec3(0.1f));
	} else {
		render::Model(model, body->GetPosition() + glm::vec3(0, -0.2f, 0), rotMatrix, glm::vec3(0.35f));
	}

}
