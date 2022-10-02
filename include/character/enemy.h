// Copyright © 2022 James Cor

#ifndef CHARACTER_ENEMY_H
#define CHARACTER_ENEMY_H

#include <raylib.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <random.h>

#define SKULL_MOVETIME 1.5

namespace character {
	// TODO: polymorphism?
	class Skull {
	public:
		Skull(glm::vec3 pos);
		~Skull() = default;

		void UpdatePosition(glm::vec3 playerPos);
		void SetTarget(glm::vec3 playerPos);
		void Draw();

	private:
		glm::mat4 model;
		float speed = 2.5f;
		float moveTime = SKULL_MOVETIME; // number of seconds before it changes direction

		// will try to stay within this donut around the player
		glm::vec3 target;
		float radMin = 3.0f;
		float radMax = 10.0f;

		// TODO: add more enemy properties
		// damage, fire rate, health
		// TODO: will need their own transformation matrix so they "look at" player
	};
}

#endif //CHARACTER_ENEMY_H
