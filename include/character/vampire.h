// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_VAMPIRE_H
#define CHARACTER_VAMPIRE_H
#include <character/enemy.h>
#include <model/raylib.h>

namespace character {
	class Vampire : public Enemy {
	public:
		Vampire(glm::vec3 pos);
		~Vampire() override = default;

		void Draw(float deltaTime) override;
		void Update(glm::vec3 playerPos, float deltaTime) override;
		void SetTarget(glm::vec3 playerPos);
		character::Projectile* Shoot();

	private:
		const float VAMPIRE_MOVETIME = 10.f;
		const float VAMPIRE_STOPTIME = 0.5f;
		const float VAMPIRE_DASH_SPEED = 100.f;
		const float VAMPIRE_WALK_SPEED = 10.f;

		glm::vec3 target;
		float moveTime = VAMPIRE_MOVETIME;
		float stopTime = VAMPIRE_STOPTIME;

		float speed = 10.0f;
		int moveState = 0;
	};
}

#endif //CHARACTER_VAMPIRE_H
