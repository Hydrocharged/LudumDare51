// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LEVEL_H
#define LEVEL_H

#include <physics/body.h>
#include <vector>
#include <memory>

namespace level {
	class Level {
	public:
		Level() {};
		~Level() = default;

		void SetDimensions(glm::vec3 dimensions) { this->dimensions = dimensions; }
		void SetPlayerSpawn(glm::vec3 playerSpawn) { this->playerSpawn = playerSpawn; }
		void SetEnemySpawns(std::vector<glm::vec3>& enemySpawns);

	private:
		glm::vec3 dimensions;
		glm::vec3 playerSpawn;
		std::vector<glm::vec3> enemySpawns;

		std::vector<std::unique_ptr<physics::Body>> bodies;

	};
}

#endif //LEVEL_H
