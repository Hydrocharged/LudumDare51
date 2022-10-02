// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_VAMPIRE_H
#define CHARACTER_VAMPIRE_H

#include <character/enemy.h>

#define VAMPIRE_MOVETIME 10
#define VAMPIRE_STOPTIME 0.5
#define VAMPIRE_DASH_SPEED 20
#define VAMPIRE_WALK_SPEED 1

namespace character {
	class Vampire : public Enemy {
	public:
		Vampire(glm::vec3 pos, Model* modelObj) : Enemy(pos, modelObj) {};
		~Vampire() = default;

		void Draw();
		void Update(glm::vec3 playerPos);
		void SetTarget(glm::vec3 playerPos);

	private:
		glm::vec3 target;
		float moveTime = VAMPIRE_MOVETIME;
		float stopTime = VAMPIRE_STOPTIME;
		bool isDashing = false;

		int moveState = 0;
	};
}

#endif //CHARACTER_VAMPIRE_H
