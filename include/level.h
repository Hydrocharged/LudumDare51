// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <character/player.h>
#include <character/enemy.h>

namespace level {
	class Level {
	public:
		Level();
		~Level() = default;

		std::shared_ptr<character::Player> GetPlayer();

		void SetDimensions(glm::vec3 dimensions) { this->dimensions = dimensions; }
		void SpawnPlayer();
		void SetPlayerSpawn(glm::vec3 playerSpawn) { this->playerSpawn = playerSpawn; }

		void SetEnemySpawns(std::vector<glm::vec3>& enemySpawns);

		enum EnemyType {
			SKULL, TURRET, VAMPIRE
		};
		void SpawnEnemy(EnemyType enemyType, unsigned int numSpawns);

		void AddBody(std::shared_ptr<physics::Body> b);

		void Draw();
		void Update();

	private:
		std::shared_ptr<Model> skullModel, turretModel, vampireModel;
		std::shared_ptr<Model> levelModel;
		std::shared_ptr<character::Player> player;

		glm::vec3 dimensions;
		glm::vec3 playerSpawn;

		std::vector<glm::vec3> enemySpawns;
		std::vector<std::shared_ptr<character::Enemy>> enemies;

		std::vector<std::shared_ptr<physics::Body>> bodies;
	};
}

#endif //LEVEL_H
