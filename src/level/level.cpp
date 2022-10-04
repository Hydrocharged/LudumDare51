// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <raylib.h>
#include <level/level.h>
#include <random.h>
#include <physics/body.h>
#include <character/skull.h>
#include <character/turret.h>
#include <character/vampire.h>
#include <model/raylib.h>

level::Level::Level() {
	this->levelModel = model::manager::Get(model::manager::Name::Level1);
}

level::Level::~Level() {
	delete player;
	for (auto enemy: enemies) {
		delete enemy;
	}
	for (auto body: bodies) {
		delete body;
	}
}

void level::Level::Draw(float deltaTime) {
	DrawModel(levelModel, (Vector3){0, -1, 0}, 10.0f, WHITE);
	player->Draw(deltaTime);
	for (auto enemy: enemies) {
		enemy->Draw(deltaTime);
	}
}

void level::Level::Update(mouse::Info& mouseInfo, float deltaTime) {
	player->UpdatePosition(mouseInfo, deltaTime);
	auto playerBody = player->GetBody();
	for (auto levelBody: bodies) {
		if (playerBody->CollidesWith(levelBody)) {
			auto movement = physics::MoveCapsuleFromAABB(*playerBody, *levelBody);
			playerBody->StopMomentum(movement);
		}
	}
	glm::vec3 playerPos = player->GetPosition();

	for (auto enemy: enemies) {
		enemy->Update(playerPos, deltaTime);
		auto enemyBody = enemy->GetBody();
		for (auto levelBody: bodies) {
			if (enemyBody->CollidesWith(levelBody)) {
				auto movement = physics::MoveCapsuleFromAABB(*enemyBody, *levelBody);
				enemyBody->StopMomentum(movement);
			}
		}
	}

	// The counter that determines death for 10 seconds
	deathTimer -= deltaTime;
	if (deathTimer <= 0.f) {
		deathTimer = 10.0f;
		for (auto enemy: enemies) {
			if (enemy->GetHealth() < 0.f) {
				enemy->Die();
			}
		}
	}
}

void level::Level::AddBody(physics::AABBBody* b) {
	bodies.push_back(b);
}

character::Player* level::Level::GetPlayer() {
	return player;
}

void level::Level::SpawnPlayer() {
	// Create player
	player = new character::Player(playerSpawn);
}

void level::Level::SetEnemySpawn(glm::vec3 spawnLocation) {
	this->enemySpawns.push_back(spawnLocation);
}

void level::Level::SpawnEnemy(character::EnemyType enemyType, unsigned int numSpawns) {
	int spawnIdx = random::GetRandomRange(0, (int)enemySpawns.size());
	for (unsigned int i = 0; i < numSpawns; i++) {
		switch (enemyType) {
			case character::EnemyType::Skull:
				enemies.push_back(new character::Skull(enemySpawns[spawnIdx]));
				break;
			case character::EnemyType::Turret:
				enemies.push_back(new character::Turret(enemySpawns[spawnIdx]));
				break;
			case character::EnemyType::Vampire:
				enemies.push_back(new character::Vampire(enemySpawns[spawnIdx]));
				break;
		}
	}
}
