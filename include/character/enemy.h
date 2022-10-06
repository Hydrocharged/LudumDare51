// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_ENEMY_H
#define CHARACTER_ENEMY_H
#include <raylib.h>
#include <physics/body.h>
#include <character/projectile.h>

namespace character {
	enum class EnemyType {
		Skull,
		Turret,
		Vampire
	};

	class Enemy {
	public:
		Enemy(physics::CapsuleBody* body, Model model);
		virtual ~Enemy() { delete body; }

		virtual void Update(glm::vec3 playerPos, float deltaTime) = 0;
		virtual void Draw(float deltaTime) = 0;
		virtual character::Projectile* Shoot() = 0;

		physics::CapsuleBody* GetBody() { return body; }
		bool CanShoot() { return cooldown <= 0.0f; }
		float GetDamage() { return damage; }
		bool CanMelee() { return meleeCooldown <= 0; }
		bool CanSpawnCrate() { return canSpawnCrate; }
		void DisableCrateSpawn() { canSpawnCrate = false; }
		void Melee() { meleeCooldown = MELEE_RATE; }
		float GetHealth() { return health; }
		float GetMaxHealth() { return maxHealth; }
		void TakeDamage(float dmg) {
			health -= dmg;
			if (health < 0) { health = 0; }
		}
		void TintModel();
		void UntintModel();

	protected:
		Model model;
		std::vector<Texture2D> textures;
		std::vector<Color> tints;
		physics::CapsuleBody* body;
		float maxHealth = 100.0f;
		float health = maxHealth;
		float damage = 5.0f;
		float cooldown = 0.0f;
		float meleeCooldown = 0.0f;
		bool canSpawnCrate = true;
		const float MELEE_RATE = 1.0f;
	};
}

#endif //CHARACTER_ENEMY_H
