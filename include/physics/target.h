// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PHYSICS_TARGET_H
#define PHYSICS_TARGET_H
#include <glm/glm.hpp>

namespace physics {
	glm::vec3 GetTargetVector(glm::vec3 position, float& angleX, float& angleY);
	void GetAngleXY(glm::vec3 position, glm::vec3 target, float& angleX, float& angleY);
}

#endif //PHYSICS_TARGET_H
