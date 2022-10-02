// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_PLAYER_H
#define CHARACTER_PLAYER_H
#include <mouse.h>
#include <memory>
#include <physics/body.h>

namespace character {
	class Player {
	public:
		Player(glm::vec3 position);
		~Player() = default;

		void UpdatePosition(mouse::Info& mouse);
		glm::vec3 GetPosition();
		physics::Body* GetBody();

		operator Camera();
		operator Camera*();

	private:
		std::unique_ptr<physics::CapsuleBody> body;
		float angleX = 0.0f;
		float angleY = 0.0f;
		std::unique_ptr<Camera> camera;
	};
}

#endif //CHARACTER_PLAYER_H
