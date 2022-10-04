// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_ENEMY_H
#define CHARACTER_ENEMY_H
#include <raylib.h>
#include <physics/body.h>

namespace character {
	enum class EnemyType {
		Skull,
		Turret,
		Vampire
	};

	class Enemy {
	public:
		Enemy(physics::CapsuleBody* body) : body(body) {}
		virtual ~Enemy() { delete body; }

		virtual void Update(glm::vec3 playerPos, float deltaTime) = 0;
		virtual void Draw(float deltaTime) = 0;
		virtual void Attack() {}
		virtual float GetHealth() { return health; }
		virtual void TakeDamage(float dmg) { health -= dmg; }
		virtual bool IsHit(physics::Body* projectile) { return body->CollidesWith(projectile); }

		void Die() { isDead = true; }
		physics::CapsuleBody* GetBody() { return body; }

	protected:
		Model model;
		physics::CapsuleBody* body;
		float health = 100.0f;
		float damage = 5.0f;
		bool isDead = false;

		// TODO: add more enemy properties
		// damage, fire rate, health
	};
}

#endif //CHARACTER_ENEMY_H
