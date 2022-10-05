// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_CRATE_H
#define CHARACTER_CRATE_H

#include <model/raylib.h>
#include <physics/body.h>
#include <memory>

namespace character {
	class Crate {
	public:
		Crate(float strength, glm::vec3 pos);
		void Update(float deltaTime);
		void Draw(float deltaTime);

		float GetStrength() { return strength; }
		physics::SphereBody* GetBody() { return body.get(); }

	private:
		Model model;
		std::unique_ptr<physics::SphereBody> body;

		float strength;
		float spin = 0.0f;
	};
}

#endif //CHARACTER_CRATE_H
