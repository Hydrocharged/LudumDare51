// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

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
		Enemy(glm::vec3 pos, Model* model);
		virtual ~Enemy() = default;

		virtual void Update(glm::vec3 playerPos) {};
		virtual void Draw() {};
		virtual void Attack() {};
		virtual void Die() {};
		virtual void TakeDamage(float dmg) {};

	protected:
		glm::mat4 model;
		float speed;
		Model* modelObj;

		float health;
		float damage;

		// TODO: add more enemy properties
		// damage, fire rate, health
	};
}

#endif //CHARACTER_ENEMY_H
