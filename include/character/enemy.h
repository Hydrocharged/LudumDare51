// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_ENEMY_H
#define CHARACTER_ENEMY_H

#include <memory>
#include <utility>

#include <raylib.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <random.h>

namespace character {
	class Enemy {
	public:
		Enemy(glm::vec3 pos, std::shared_ptr<Model> model);
		virtual ~Enemy() = default;

		virtual void Update(glm::vec3 playerPos) {};
		virtual void Draw() {};

		virtual float GetHealth() { return health; }
		virtual void Attack() {};

		virtual void TakeDamage(float dmg) { health -= dmg; }
		virtual void Die() { isDead = true; }

	protected:
		std::shared_ptr<Model> modelObj;

		glm::mat4 model;
		float speed;

		float health;
		float damage;
		bool isDead = false;

		// TODO: add more enemy properties
		// damage, fire rate, health
	};
}

#endif //CHARACTER_ENEMY_H
