// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef CAMERA_CAMERA_H
#define CAMERA_CAMERA_H

#include <raylib.h>
#include <memory>

namespace player {
	class Player {
	public:
		Player(float x, float y, float z);
		~Player() = default;

		Camera* GetCamera();
	private:
		std::unique_ptr<Camera> camera;
	};
}


#endif //CAMERA_CAMERA_H
