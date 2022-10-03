// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/enemy.h>

character::Enemy::Enemy(glm::vec3 pos, std::shared_ptr<Model> modelObj) {
	model = glm::translate(glm::mat4(1.0f), pos);
	this->modelObj = std::move(modelObj);
}