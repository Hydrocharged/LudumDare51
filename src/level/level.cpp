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
#include <render/render.h>

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
	for (auto projectile: projectiles) {
		projectile->Draw(deltaTime);
	}
	for (auto crate: crates) {
		crate->Draw(deltaTime);
	}
}

void level::Level::Update(mouse::Info& mouseInfo, float deltaTime) {
	// Check player to level collisions
	player->UpdatePosition(mouseInfo, deltaTime);
	auto playerBody = player->GetBody();
	for (auto levelBody: bodies) {
		if (playerBody->CollidesWith(levelBody)) {
			auto movement = physics::MoveCapsuleFromAABB(*playerBody, *levelBody);
			playerBody->StopMomentum(movement);
		}
	}

	// Check enemy to level collisions
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

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		for (auto projectile: player->Shoot()) {
			projectiles.insert(projectile);
		}
	}

	// Update projectile
	std::vector<character::Projectile*> toDeleteProjectiles;
	for (auto projectile: projectiles) {
		bool wasDeleted = false;
		projectile->Update(deltaTime);
		if (projectile->GetLifeSpan() <= 0.0f) {
			wasDeleted = true;
			toDeleteProjectiles.push_back(projectile);
		}
		if (wasDeleted) {
			continue;
		}

		auto projectileBody = projectile->GetBody();

		// check projectile to level collisions
		for (auto levelBody: bodies) {
			if (projectileBody->CollidesWith(levelBody)) {
				wasDeleted = true;
				toDeleteProjectiles.push_back(projectile);
			}
		}

		if (wasDeleted) {
			continue;
		}

		// check projectile to enemy collisions
		for (auto enemy: enemies) {
			auto enemyBody = enemy->GetBody();
			if (projectile->IsFromPlayer() && projectileBody->CollidesWith(enemyBody)) {
				enemy->TakeDamage(projectile->GetDamage());
				wasDeleted = true;
				toDeleteProjectiles.push_back(projectile);
			}
		}

		if (wasDeleted) {
			continue;
		}

		// check projectile to player collision
		if (!projectile->IsFromPlayer() && projectileBody->CollidesWith(playerBody)) {
			player->TakeDamage(projectile->GetDamage());
			toDeleteProjectiles.push_back(projectile);
		}
	}
	for (auto projectile: toDeleteProjectiles) {
		projectiles.erase(projectile);
	}

	// Update crates
	std::vector<character::Crate*> toDeleteCrates;
	for (auto crate: crates) {
		crate->Update(deltaTime);
		auto crateBody = crate->GetBody();
		if (crateBody->CollidesWith(playerBody)) {
			toDeleteCrates.push_back(crate);
			player->AddAmmo(crate->GetAmmo());
			player->AddHealth(crate->GetHealth());
		}
		for (auto levelBody: bodies) {
			if (crateBody->CollidesWith(levelBody)) {
				auto movement = physics::MoveSphereFromAABB(*crateBody, *levelBody);
				crateBody->StopMomentum(movement);
			}
		}
	}
	for (auto crate: toDeleteCrates) {
		crates.erase(crate);
	}

	// The counter that determines death for 10 seconds
	deathTimer -= deltaTime;
	if (deathTimer <= 0.f) {
		deathTimer = DEATH_TIME;
		// Kill enemies
		std::vector<character::Enemy*> toDeleteEnemies;
		for (auto enemy: enemies) {
			if (enemy->GetHealth() < 0.f) {
				toDeleteEnemies.push_back(enemy);
				crates.emplace(new character::Crate(10, 10, enemy->GetBody()->GetPosition()));
			}
		}
		for (auto enemy: toDeleteEnemies) {
			enemies.erase(enemy);
			delete enemy;
		}

		// Kill player
		if (player->GetHealth() <= 0.f) {
			gameOver();
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
				enemies.emplace(new character::Skull(enemySpawns[spawnIdx]));
				break;
			case character::EnemyType::Turret:
				enemies.emplace(new character::Turret(enemySpawns[spawnIdx]));
				break;
			case character::EnemyType::Vampire:
				enemies.emplace(new character::Vampire(enemySpawns[spawnIdx]));
				break;
		}
	}
}
