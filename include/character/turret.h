// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_TURRET_H
#define CHARACTER_TURRET_H
#include <character/enemy.h>
#include <model/raylib.h>

namespace character {
	class Turret : public Enemy {
	public:
		Turret(glm::vec3 pos);
		~Turret() override = default;

		void Draw(float deltaTime) override;
		void Update(glm::vec3 playerPos, float deltaTime) override;
		void SetTarget(glm::vec3 playerPos);
		character::Projectile* Shoot();

	private:
		const float TURRET_MOVETIME = 10.f;
		float moveTime = TURRET_MOVETIME;
		glm::vec3 playerLoc;

		// will try to stay within this donut around the player
		glm::vec3 target{0};
		float radMin = 5.0f;
		float radMax = 7.0f;

		const float TURRET_FIRE_RATE = 4.0f;
	};
}

#endif //CHARACTER_TURRET_H
