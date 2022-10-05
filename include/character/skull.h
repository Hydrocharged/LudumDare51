// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_SKULL_H
#define CHARACTER_SKULL_H
#include <character/enemy.h>
#include <model/raylib.h>
#include <character/projectile.h>

namespace character {
	class Skull : public Enemy {
	public:
		Skull(glm::vec3 pos);
		~Skull() override = default;

		void Draw(float deltaTime) override;
		void Update(glm::vec3 playerPos, float deltaTime) override;
		void SetTarget(glm::vec3 playerPos);
		character::Projectile* Shoot();
		bool CanShoot();

	private:
		// Number of seconds before it changes direction
		const float SKULL_MOVETIME = 1.5f;
		float moveTime = SKULL_MOVETIME;

		// Will try to stay within this donut around the player
		glm::vec3 target{0};
		float speed = 50.0f;
		float radMin = 3.0f;
		float radMax = 10.0f;

		const float SKULL_FIRE_RATE = 1.5f;
		float fireCoolDown = SKULL_FIRE_RATE;
	};
}

#endif //CHARACTER_SKULL_H
