// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_VAMPIRE_H
#define CHARACTER_VAMPIRE_H

#include <character/enemy.h>

namespace character {
	class Vampire : public Enemy {
	public:
		Vampire(glm::vec3 pos, Model* modelObj) : Enemy(pos, modelObj) {};
		~Vampire() = default;

		void Draw();
		void Update(glm::vec3 playerPos);
		void SetTarget(glm::vec3 playerPos);

	private:
		const float VAMPIRE_MOVETIME = 10.f;
		const float VAMPIRE_STOPTIME = 0.5f;
		const float VAMPIRE_DASH_SPEED = 20.f;
		const float VAMPIRE_WALK_SPEED = 1.f;

		glm::vec3 target;
		float moveTime = VAMPIRE_MOVETIME;
		float stopTime = VAMPIRE_STOPTIME;

		int moveState = 0;
	};
}

#endif //CHARACTER_VAMPIRE_H
