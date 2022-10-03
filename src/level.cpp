// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <level.h>

level::Level::Level() {
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
	for (std::shared_ptr<character::Enemy> enemy: enemies) {
		enemy->Draw();
	}
}

void level::Level::Update() {
	// TODO: figure out how pointers work
	//glm::vec3 playerPos = player->GetPosition();

	for (auto enemy: enemies) {
		enemy->Update(playerPos);
	}

	// TODO: update other stuff
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