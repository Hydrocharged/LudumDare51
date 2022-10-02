// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_SKULL_H
#define CHARACTER_SKULL_H

#include <character/enemy.h>

namespace character {
	class Skull : public Enemy {
	public:
		Skull(glm::vec3 pos, std::shared_ptr<Model> modelObj) : Enemy(pos, modelObj) {
			speed = 2.5f;
		};
		~Skull() = default;

		void Draw();
		void Update(glm::vec3 playerPos);

		void SetTarget(glm::vec3 playerPos);

	private:
		// number of seconds before it changes direction
		const float SKULL_MOVETIME = 1.5f;
		float moveTime = SKULL_MOVETIME;

		// will try to stay within this donut around the player
		glm::vec3 target;
		float radMin = 3.0f;
		float radMax = 10.0f;
	};
}

#endif //CHARACTER_SKULL_H
