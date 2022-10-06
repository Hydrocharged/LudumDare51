// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_PROJECTILE_H
#define CHARACTER_PROJECTILE_H
#include <physics/body.h>
#include <memory>
#include <raylib.h>

namespace character {
	enum class ProjectileType {
		Pistol,
		Shotgun,
		Sniper,
		Enemy
	};

	class Projectile {
	public:
		Projectile(bool fromPlayer, float speed, float size, float damage, float lifeSpan, glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix);
		virtual ~Projectile() { delete body; }
		void Update(float deltaTime);
		void Draw(float deltaTime);

		physics::SphereBody* GetBody() { return body; }
		float GetDamage() { return damage; }
		float GetLifeSpan() { return lifeSpan; }
		virtual bool IsFromPlayer() = 0;
		virtual ProjectileType Type() = 0;

	private:
		Model model;
		physics::SphereBody* body;
		glm::mat4 rotMatrix;

		float damage;
		float lifeSpan;
	};

	class PistolProjectile : public Projectile {
	public:
		PistolProjectile(float damage, glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix);

		bool IsFromPlayer() override { return true; }
		ProjectileType Type() override { return ProjectileType::Pistol; }
	};

	class ShotgunProjectile : public Projectile {
	public:
		ShotgunProjectile(float damage, glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix);

		bool IsFromPlayer() override { return true; }
		ProjectileType Type() override { return ProjectileType::Shotgun; }
	};

	class SniperProjectile : public Projectile {
	public:
		SniperProjectile(float damage, glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix);

		bool IsFromPlayer() override { return true; }
		ProjectileType Type() override { return ProjectileType::Sniper; }
	};

	class SkullProjectile : public Projectile {
	public:
		SkullProjectile(glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix);

		bool IsFromPlayer() override { return false; }
		ProjectileType Type() override { return ProjectileType::Enemy; }
	};

	class TurretProjectile : public Projectile {
	public:
		TurretProjectile(glm::vec3 pos, glm::vec3 dir, glm::mat4 rotMatrix);

		bool IsFromPlayer() override { return false; }
		ProjectileType Type() override { return ProjectileType::Enemy; }
	};
}

#endif //CHARACTER_PROJECTILE_H
