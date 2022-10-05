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
#include <stats/frame.h>

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
	if(isPaused || isGameOver) {
		deltaTime = 0.0f;
	}
	DrawModel(levelModel, (Vector3){0, -1, 0}, 10.0f, WHITE);
	for (auto enemy: enemies) {
		enemy->Draw(deltaTime);
	}
	for (auto projectile: projectiles) {
		projectile->Draw(deltaTime);
	}
	for (auto crate: crates) {
		crate->Draw(deltaTime);
	}
	player->Draw(deltaTime);
}

void level::Level::Update(mouse::Info& mouseInfo, float deltaTime) {
	if(isPaused || isGameOver) {
		return;
	}
	// Get frame stats to update
	auto frameStats = stats::Frame::Current();
	// Check player to level collisions
	player->UpdatePosition(mouseInfo, deltaTime);
	auto playerBody = player->GetBody();
	for (auto levelBody: bodies) {
		frameStats->PlayerCollisionChecks++;
		frameStats->LevelCollisionChecks++;
		if (playerBody->CollidesWith(levelBody)) {
			frameStats->PlayerCollisions++;
			frameStats->LevelCollisions++;
			auto movement = physics::MoveCapsuleFromAABB(*playerBody, *levelBody);
			playerBody->StopMomentum(movement);
		}
	}

	glm::vec3 playerPos = player->GetPosition();
	for (auto enemy: enemies) {
		enemy->Update(playerPos, deltaTime);

		// Make enemies shoot
		if (enemy->CanShoot()) {
			auto projectile = enemy->Shoot();
			if (projectile != nullptr) { projectiles.emplace(projectile); }
		}

		// Check enemy to level collisions
		auto enemyBody = enemy->GetBody();
		for (auto levelBody: bodies) {
			frameStats->EnemyCollisionChecks++;
			frameStats->LevelCollisionChecks++;
			if (enemyBody->CollidesWith(levelBody)) {
				frameStats->EnemyCollisions++;
				frameStats->LevelCollisions++;
				auto movement = physics::MoveCapsuleFromAABB(*enemyBody, *levelBody);
				enemyBody->StopMomentum(movement);
			}
		}

		// check enemy to player collision
		if (enemyBody->CollidesWith(playerBody)) {
			if (enemy->CanMelee()) {
				enemy->Melee();
				player->TakeDamage(enemy->GetDamage());
			}
		}
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (player->CanShoot()) {
			for (auto projectile: player->Shoot()) {
				projectiles.insert(projectile);
			}
		}
	}


	// Update projectile
	std::set<character::Projectile*> toDeleteProjectiles;
	for (auto projectile: projectiles) {
		bool wasDeleted = false;
		projectile->Update(deltaTime);
		if (projectile->GetLifeSpan() <= 0.0f) {
			wasDeleted = true;
			toDeleteProjectiles.emplace(projectile);
		}
		if (wasDeleted) {
			continue;
		}

		auto projectileBody = projectile->GetBody();

		// check projectile to level collisions
		for (auto levelBody: bodies) {
			frameStats->ProjectileCollisionChecks++;
			frameStats->LevelCollisionChecks++;
			if (projectileBody->CollidesWith(levelBody)) {
				frameStats->ProjectileCollisions++;
				frameStats->LevelCollisions++;
				wasDeleted = true;
				toDeleteProjectiles.emplace(projectile);
			}
		}

		if (wasDeleted) {
			continue;
		}

		// check projectile to enemy collisions
		for (auto enemy: enemies) {
			auto enemyBody = enemy->GetBody();
			if (projectile->IsFromPlayer()) {
				frameStats->ProjectileCollisionChecks++;
				frameStats->EnemyCollisionChecks++;
				if (projectileBody->CollidesWith(enemyBody)) {
					frameStats->ProjectileCollisions++;
					frameStats->EnemyCollisions++;
					enemy->TakeDamage(projectile->GetDamage());
					wasDeleted = true;
					toDeleteProjectiles.emplace(projectile);
					if(enemy->GetHealth() <= 0.0f && enemy->CanSpawnCrate()) {
						score += 5 + (uint64_t)(10.0f - deathTimer);
						enemy->DisableCrateSpawn();
						auto spawnedCrate = new character::Crate(10.0f + (float)floor(totalTime / 10.0) * 3.0f, enemy->GetBody()->GetPosition());
						crates.emplace(spawnedCrate);
						spawnedCrate->GetBody()->ApplyInstantForce(glm::normalize(glm::vec3{random::GetRandomRange(-0.4f, 0.4f), 0.5f, random::GetRandomRange(-0.4f, 0.4f)}), 10.0f);
					}
				}
			}
		}

		if (wasDeleted) {
			continue;
		}

		// check projectile to player collision
		if (!projectile->IsFromPlayer()) {
			frameStats->ProjectileCollisionChecks++;
			frameStats->PlayerCollisionChecks++;
			if (projectileBody->CollidesWith(playerBody)) {
				frameStats->ProjectileCollisions++;
				frameStats->PlayerCollisions++;
				player->TakeDamage(projectile->GetDamage());
				toDeleteProjectiles.emplace(projectile);
			}
		}

		// TODO: somewhat expensive, could separate enemy and player projectiles
		// check projectile to projectile collision
		for (auto projectile2: projectiles) {
			if (projectile == projectile2) {
				continue;
			}

			if (projectile->IsFromPlayer() != projectile2->IsFromPlayer()) {
				if (projectileBody->CollidesWith(projectile2->GetBody())) {
					toDeleteProjectiles.emplace(projectile);
					toDeleteProjectiles.emplace(projectile2);
				}
			}
		}
	}
	for (auto projectile: toDeleteProjectiles) {
		projectiles.erase(projectile);
	}

	// Update crates
	std::vector<character::Crate*> toDeleteCrates;
	for (auto crate: crates) {
		frameStats->ItemCollisionChecks++;
		frameStats->PlayerCollisionChecks++;
		crate->Update(deltaTime);
		auto crateBody = crate->GetBody();
		if (crateBody->CollidesWith(playerBody)) {
			frameStats->ItemCollisions++;
			frameStats->PlayerCollisions++;
			toDeleteCrates.push_back(crate);
			if(IsKeyDown(KEY_Q)) {
				player->AddHealth(crate->GetStrength() * 2.5f);
			} else if(IsKeyDown(KEY_E)) {
				player->AddAmmo(crate->GetStrength() * 2.5f);
			} else {
				player->AddAmmo(crate->GetStrength());
				player->AddHealth(crate->GetStrength());
			}
		}
		for (auto levelBody: bodies) {
			frameStats->ItemCollisionChecks++;
			frameStats->LevelCollisionChecks++;
			if (crateBody->CollidesWith(levelBody)) {
				frameStats->ItemCollisions++;
				frameStats->LevelCollisions++;
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
		// Kill crates
		for(auto crate: crates) {
			delete crate;
		}
		crates.clear();

		// Kill enemies
		std::vector<character::Enemy*> toDeleteEnemies;
		for (auto enemy: enemies) {
			if (enemy->GetHealth() <= 0.f) {
				toDeleteEnemies.push_back(enemy);
			}
		}
		for (auto enemy: toDeleteEnemies) {
			enemies.erase(enemy);
			delete enemy;
		}

		auto spawnAmounts = random::GetRandomDistribution<3>(2.0 + floor((totalTime / 10.0)));
		SpawnEnemy(character::EnemyType::Skull, (unsigned int)round(spawnAmounts[0]));
		SpawnEnemy(character::EnemyType::Turret, (unsigned int)round(spawnAmounts[1]));
		SpawnEnemy(character::EnemyType::Vampire, (unsigned int)round(spawnAmounts[2]));

		// Kill player
		if (player->GetHealth() <= 0.f) {
			GameOver();
		}
	}
	totalTime += deltaTime;
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
