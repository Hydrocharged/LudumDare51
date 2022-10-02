// Copyright © 2022 James Cor

#ifndef CHARACTER_ENEMY_H
#define CHARACTER_ENEMY_H

#include <raylib.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <random.h>


namespace character {
	class Enemy {
	public:
		Enemy(glm::vec3 pos);
		virtual ~Enemy() = default;

		virtual void Update(glm::vec3 playerPos){};
		virtual void Draw(){};
		virtual void Attack(){};

	protected:
		glm::mat4 model;
		float speed;

		float health;
		float damage;


		// TODO: add more enemy properties
		// damage, fire rate, health
	};
}

#endif //CHARACTER_ENEMY_H
