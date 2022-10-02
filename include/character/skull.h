// Copyright © 2022 James Cor

#ifndef CHARACTER_SKULL_H
#define CHARACTER_SKULL_H

#include <character/enemy.h>

#define SKULL_MOVETIME 1.5

namespace character {
	class Skull : public Enemy {
	public:
		Skull(glm::vec3 pos) : Enemy(pos) {
			speed = 2.5f;
		};
		~Skull() = default;

		void Draw();
		void Update(glm::vec3 playerPos);
		void SetTarget(glm::vec3 playerPos);

	private:
		// number of seconds before it changes direction
		float moveTime = SKULL_MOVETIME;

		// will try to stay within this donut around the player
		glm::vec3 target;
		float radMin = 3.0f;
		float radMax = 10.0f;
	};
}

#endif //CHARACTER_SKULL_H
