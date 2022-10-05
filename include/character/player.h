// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_PLAYER_H
#define CHARACTER_PLAYER_H
#include <mouse.h>
#include <memory>
#include <physics/body.h>
#include <character/projectile.h>

namespace character {
	class Player {
	public:
		Player(glm::vec3 position);
		~Player() = default;

		void UpdatePosition(mouse::Info& mouse, float deltaTime);
		void Draw(float deltaTime);
		enum WeaponType {
			PISTOL,
			SHOTGUN,
			SNIPER,
		};
		void SetCurrentWeapon(WeaponType weapon);

		glm::vec3 GetPosition();
		glm::vec3 GetCameraPosition();
		physics::CapsuleBody* GetBody();
		float GetHealth() { return health; }

		void AddHealth(float health) { this->health += health; }
		void AddAmmo(float ammo) { this->ammo += ammo; }
		void TakeDamage(float dmg) { health -= dmg; }
		bool CanShoot();

		std::vector<Projectile*> Shoot();

		operator ::Camera();
		operator ::Camera*();

	private:
		std::unique_ptr<physics::CapsuleBody> body;
		float moveSpeedConstant = 40.0f;
		float sprintModifier = 3.0f;
		float dashModifier = 1.2f;
		float jumpForce = 15.0f;
		float mouseSensitivity = 0.5f;
		float health = 100.0f;
		float ammo = 100.0f;
		std::unique_ptr<Camera> camera;
		Model pistol, shotgun, sniper;
		WeaponType currentWeapon = PISTOL;

		const float PISTOL_AMMO = 1;
		const float SHOTGUN_AMMO = 10;
		const float SNIPER_AMMO = 5;

		const float PISTOL_FIRE_RATE = 0.5;
		const float SHOTGUN_FIRE_RATE = 1;
		const float SNIPER_FIRE_RATE = 1;
		float pistolCooldown = 0;
		float shotgunCooldown = 0;
		float sniperCooldown = 0;

	};
}

#endif //CHARACTER_PLAYER_H
