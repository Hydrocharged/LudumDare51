// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/projectile.h>
#include <render/model.h>
#include <model/raylib.h>

character::Projectile::Projectile(bool fromPlayer, float speed, float size, float damage, float lifeSpan, glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix) {
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
	if (IsFromPlayer()) {
		render::Model(model, body, rotMatrix, glm::vec3(0.1f));
	} else {
		render::Model(model, body->GetPosition() + glm::vec3(0, -0.2f, 0), rotMatrix, glm::vec3(0.35f));
	}
}

character::PistolProjectile::PistolProjectile(float damage, glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix) :
	Projectile(true, 40.0f, 0.2f, damage, 2.0f, pos, dir, rotMatrix) {}

character::ShotgunProjectile::ShotgunProjectile(float damage, glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix) :
	Projectile(true, 25.0f, 0.2f, damage, 1.2f, pos, dir, rotMatrix) {}

character::SniperProjectile::SniperProjectile(float damage, glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix) :
	Projectile(true, 50.0f, 0.2f, damage, 3.0f, pos, dir, rotMatrix) {}

character::SkullProjectile::SkullProjectile(glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix) :
	Projectile(false, 10.0f, 0.2f, 7, 10.0f, pos, dir, rotMatrix) {}

character::TurretProjectile::TurretProjectile(glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix) :
	Projectile(false, 5.0f, 0.2f, 10, 10.0f, pos, dir, rotMatrix) {}
