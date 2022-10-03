// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <raylib.h>
#include <level.h>
#include <render/render.h>
#include <random.h>
#include <physics/body.h>
#include <character/skull.h>
#include <character/turret.h>
#include <character/vampire.h>

level::Level::Level() {
	// Load level model
	Model levelModel = LoadModel("../assets/models/level1/level1.obj");
	Texture2D levelTexture = LoadTexture("../assets/models/level1/level1.png");
	SetMaterialTexture(&levelModel.materials[0], MATERIAL_MAP_DIFFUSE, levelTexture);
	this->levelModel = std::make_unique<Model>(levelModel);

	// Load enemy models
	Model skullModel = LoadModel("../assets/models/skull/skull.obj");
	Texture2D skullTexture = LoadTexture("../assets/models/skull/skull.png");
	SetMaterialTexture(&skullModel.materials[0], MATERIAL_MAP_DIFFUSE, skullTexture);
	this->skullModel = std::make_unique<Model>(skullModel);

	Model turretModel = LoadModel("../assets/models/turret/turret.obj");
	Texture2D turretTexture = LoadTexture("../assets/models/turret/turret.png");
	SetMaterialTexture(&turretModel.materials[0], MATERIAL_MAP_DIFFUSE, turretTexture);
	this->turretModel = std::make_unique<Model>(turretModel);

	Model vampireModel = LoadModel("../assets/models/vampire/vampire.obj");
	Texture2D vampireTexture = LoadTexture("../assets/models/vampire/vampire.png");
	SetMaterialTexture(&vampireModel.materials[0], MATERIAL_MAP_DIFFUSE, vampireTexture);
	this->vampireModel = std::make_unique<Model>(vampireModel);
}

void level::Level::Draw() {
	DrawModel(*levelModel, (Vector3){0, 0, 0}, 10.0f, WHITE);
	for (std::shared_ptr<character::Enemy> enemy: enemies) {
		enemy->Draw();
	}
}

void level::Level::Update() {
	glm::vec3 playerPos = player->GetPosition();
	for (auto enemy: enemies) {
		enemy->Update(playerPos);
	}

	// The counter that determines death for 10 seconds
	deathTimer -= GetFrameTime();
	if (deathTimer <= 0.f) {
		deathTimer = DEATH_TIME;
		for (auto enemy: enemies) {
			if (enemy->GetHealth() < 0.f) {
				enemy->Die();
			}
		}
	}
}

void level::Level::AddBody(std::shared_ptr<physics::Body> b) {
	bodies.push_back(b);
}

std::shared_ptr<character::Player> level::Level::GetPlayer() {
	return player;
}

void level::Level::SpawnPlayer() {
	// Create player
	player = std::make_shared<character::Player>(playerSpawn);
}

void level::Level::SetEnemySpawns(std::vector<glm::vec3>& enemySpawns) {
	this->enemySpawns = std::vector<glm::vec3>(enemySpawns);
}

void level::Level::SpawnEnemy(EnemyType enemyType, unsigned int numSpawns) {
	int spawnIdx = random::GetRandomRange(0, (int)enemySpawns.size());
	for (unsigned int i = 0; i < numSpawns; i++) {
		switch (enemyType) {
			case SKULL:
				enemies.push_back(std::make_unique<character::Skull>(enemySpawns[spawnIdx], skullModel));
				break;
			case TURRET:
				enemies.push_back(std::make_unique<character::Turret>(enemySpawns[spawnIdx], turretModel));
				break;
			case VAMPIRE:
				enemies.push_back(std::make_unique<character::Vampire>(enemySpawns[spawnIdx], vampireModel));
				break;
		}
	}
}