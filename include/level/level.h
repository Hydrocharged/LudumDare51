// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <set>
#include <mouse.h>
#include <character/player.h>
#include <character/enemy.h>
#include <character/projectile.h>
#include <character/crate.h>

namespace level {
	class Level {
	public:
		Level();
		~Level();

		character::Player* GetPlayer();

		void SetDimensions(glm::vec3 dimensions) { this->dimensions = dimensions; }
		void SpawnPlayer();
		void SetPlayerSpawn(glm::vec3 playerSpawn) { this->playerSpawn = playerSpawn; }

		void SetEnemySpawn(glm::vec3 spawnLocation);
		void SpawnEnemy(character::EnemyType enemyType, unsigned int numSpawns);

		void AddBody(physics::AABBBody* b);

		void Draw(float deltaTime);
		void Update(mouse::Info& mouseInfo, float deltaTime);
		bool IsPaused() { return isPaused; }
		bool IsGameOver() { return isGameOver; }
		void Pause() { isPaused = true; EnableCursor(); }
		void Unpause() { isPaused = false; DisableCursor(); }
		double GetTotalTime() { return totalTime; }
		float GetDeathTimer() { return deathTimer; }
		uint64_t GetScore() { return score; }

	private:
		const float DEATH_TIME = 10.f;
		float deathTimer = DEATH_TIME;
		double totalTime = 0.0;
		uint64_t score = 0;

		bool isPaused = false;
		bool isGameOver = false;
		Model levelModel;
		character::Player* player;

		glm::vec3 dimensions;
		glm::vec3 playerSpawn;

		std::vector<physics::AABBBody*> bodies;
		std::vector<glm::vec3> enemySpawns;
		std::set<character::Enemy*> enemies;
		std::set<character::Projectile*> playerProjectiles;
		std::set<character::Projectile*> enemyProjectiles;
		std::set<character::Crate*> crates;

		void GameOver() { isGameOver = true; EnableCursor(); }
	};

	std::unique_ptr<Level> GetLevel1();
}

#endif //LEVEL_H
