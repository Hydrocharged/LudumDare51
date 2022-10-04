// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_PROJECTILE_H
#define CHARACTER_PROJECTILE_H
#include <physics/body.h>
#include <memory>
#include <raylib.h>

namespace character {
	class Projectile {
	public:
		Projectile(bool fromPlayer, float speed, float size, float damage, float lifeSpan, glm::vec3 pos, glm::vec3 dir);
		void Update(float deltaTime);
		void Draw(float deltaTime);

		bool IsFromPlayer() { return fromPlayer; }
		physics::SphereBody* GetBody() { return body.get(); }
		float GetDamage() { return damage; }
		float GetLifeSpan() { return lifeSpan; }

	private:
		Model model;

		std::unique_ptr<physics::SphereBody> body;

		float damage;
		float lifeSpan;
		bool fromPlayer;
	};
}

#endif //CHARACTER_PROJECTILE_H
