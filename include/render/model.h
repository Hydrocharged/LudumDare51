// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef RENDER_RAYLIB_H
#define RENDER_RAYLIB_H
#include <raylib.h>
#include <physics/body.h>

namespace render {
	void Model(::Model model, physics::Body* body, glm::vec3 scale);
	void Model(::Model model, physics::Body* body, glm::vec3 renderPosition, glm::vec3 scale);
}

#endif //RENDER_RAYLIB_H
