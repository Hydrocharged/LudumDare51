// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_TURRENT_H
#define CHARACTER_TURRENT_H

#include <character/enemy.h>

#define TURRENT_MOVETIME 5

namespace character {
	class Turret : public Enemy {
	public:
		Turret(glm::vec3 pos, Model* modelObj) : Enemy(pos, modelObj) {};
		~Turret() = default;

		void Draw();
		void Update(glm::vec3 playerPos);

		glm::vec3 FindTarget(glm::vec3 playerPos);

	private:
		float moveTime = TURRENT_MOVETIME;

		// will try to stay within this donut around the player
		float radMin = 1.0f;
		float radMax = 2.5f;
	};
}

#endif //CHARACTER_TURRENT_H
