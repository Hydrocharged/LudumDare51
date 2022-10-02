// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CHARACTER_PLAYER_H
#define CHARACTER_PLAYER_H

#include <raylib.h>
#include <mouse.h>
#include <memory>

namespace character {
	class Player {
	public:
		Player(float x, float y, float z);
		~Player() = default;

		void UpdatePosition(mouse::Info& mouse);
		Vector3 GetPosition();

		operator Camera();
		operator Camera*();

	private:
		float angleX = 0.0f;
		float angleY = 0.0f;
		std::unique_ptr<Camera> camera;
	};
}

#endif //CHARACTER_PLAYER_H
