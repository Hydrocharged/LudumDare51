// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <level.h>

void level::Level::SetEnemySpawns(std::vector<glm::vec3>& enemySpawns) {
	this->enemySpawns = std::vector<glm::vec3>(enemySpawns);
}

