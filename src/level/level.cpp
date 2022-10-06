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
	isPaused = true;
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
	if (isPaused || isGameOver) {
		deltaTime = 0.0f;
	}
	DrawModel(levelModel, (Vector3){0, -1, 0}, 10.0f, WHITE);
	for (auto enemy: enemies) {
		enemy->Draw(deltaTime);
	}
	for (auto projectile: enemyProjectiles) {
		projectile->Draw(deltaTime);
	}
	for (auto projectile: playerProjectiles) {
		projectile->Draw(deltaTime);
	}
	for (auto crate: crates) {
		crate->Draw(deltaTime);
	}
	player->Draw(deltaTime);
}

void level::Level::Update(mouse::Info& mouseInfo, float deltaTime) {
	if (isPaused || isGameOver) {
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
			if (projectile != nullptr) {
				enemyProjectiles.emplace(projectile);
			}
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

		// Check enemy to player collision
		if (enemyBody->CollidesWith(playerBody)) {
			if (enemy->CanMelee()) {
				enemy->Melee();
				player->TakeDamage(enemy->GetDamage(), glm::normalize(playerBody->GetPosition() - enemyBody->GetPosition()));
			}
		}
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (player->CanShoot()) {
			for (auto projectile: player->Shoot()) {
				playerProjectiles.emplace(projectile);
			}
		}
	}

	// Update projectile
	std::set<character::Projectile*> toDeleteProjectiles;
	for (auto playerProjectile: playerProjectiles) {
		bool wasDeleted = false;
		playerProjectile->Update(deltaTime);
		if (playerProjectile->GetLifeSpan() <= 0.0f) {
			wasDeleted = true;
			toDeleteProjectiles.emplace(playerProjectile);
		}
		if (wasDeleted) {
			continue;
		}

		auto projectileBody = playerProjectile->GetBody();
		// Check projectile to level collisions
		for (auto levelBody: bodies) {
			frameStats->ProjectileCollisionChecks++;
			frameStats->LevelCollisionChecks++;
			if (projectileBody->CollidesWith(levelBody)) {
				frameStats->ProjectileCollisions++;
				frameStats->LevelCollisions++;
				wasDeleted = true;
				toDeleteProjectiles.emplace(playerProjectile);
				break;
			}
		}
		if (wasDeleted) {
			continue;
		}

		// Check projectile to enemy collisions
		for (auto enemy: enemies) {
			auto enemyBody = enemy->GetBody();
			frameStats->ProjectileCollisionChecks++;
			frameStats->EnemyCollisionChecks++;
			if (projectileBody->CollidesWith(enemyBody)) {
				frameStats->ProjectileCollisions++;
				frameStats->EnemyCollisions++;
				float projectileDamage = playerProjectile->GetDamage();
				playerProjectile->DecreaseDamage(enemy->GetHealth());
				enemy->TakeDamage(projectileDamage);
				if (playerProjectile->GetDamage() <= 0.0f) {
					wasDeleted = true;
					toDeleteProjectiles.emplace(playerProjectile);
				}
				if (enemy->GetHealth() <= 0.0f && enemy->CanSpawnCrate()) {
					score += 5 + (uint64_t)(10.0f - deathTimer);
					enemy->DisableCrateSpawn();
					if (deathTimer >= 1.0f) {
						auto spawnedCrate = new character::Crate(8.0f, enemy->GetBody()->GetPosition());
						crates.emplace(spawnedCrate);
						spawnedCrate->GetBody()->ApplyInstantForce(glm::normalize(glm::vec3{rando::GetRandomRange(-0.5f, 0.5f), 0.25f, rando::GetRandomRange(-0.5f, 0.5f)}), 15.0f);
					}
				}
				if (wasDeleted) {
					break;
				}
			}
		}
		if (wasDeleted) {
			continue;
		}

		// Check player projectile to enemy projectile collision
		for (auto enemyProjectile: enemyProjectiles) {
			if (projectileBody->CollidesWith(enemyProjectile->GetBody())) {
				toDeleteProjectiles.emplace(enemyProjectile);
				if (playerProjectile->Type() != character::ProjectileType::Sniper) {
					playerProjectile->DecreaseDamage(20.0f);
					if (playerProjectile->GetDamage() <= 0.0f) {
						toDeleteProjectiles.emplace(playerProjectile);
						break;
					}
				}
			}
		}
	}

	for (auto projectile: toDeleteProjectiles) {
		if (projectile->IsFromPlayer()) {
			playerProjectiles.erase(projectile);
		} else {
			enemyProjectiles.erase(projectile);
		}
		delete projectile;
	}
	toDeleteProjectiles.clear();

	// Update remaining enemy projectiles
	for (auto enemyProjectile: enemyProjectiles) {
		enemyProjectile->Update(deltaTime);
		auto projectileBody = enemyProjectile->GetBody();
		// Check enemy projectile to player
		frameStats->ProjectileCollisionChecks++;
		frameStats->PlayerCollisionChecks++;
		if (projectileBody->CollidesWith(playerBody)) {
			frameStats->ProjectileCollisions++;
			frameStats->PlayerCollisions++;
			player->TakeDamage(enemyProjectile->GetDamage(), glm::vec3(0.0f));
			toDeleteProjectiles.emplace(enemyProjectile);
			break;
		}

		// Check enemy projectile to level
		for (auto levelBody: bodies) {
			frameStats->ProjectileCollisionChecks++;
			frameStats->LevelCollisionChecks++;
			if (projectileBody->CollidesWith(levelBody)) {
				frameStats->ProjectileCollisions++;
				frameStats->LevelCollisions++;
				toDeleteProjectiles.emplace(enemyProjectile);
				break;
			}
		}
	}

	// Delete the rest of the projectiles from the next check
	for (auto projectile: toDeleteProjectiles) {
		if (projectile->IsFromPlayer()) {
			playerProjectiles.erase(projectile);
		} else {
			enemyProjectiles.erase(projectile);
		}
		delete projectile;
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
			if (IsKeyDown(KEY_Q)) {
				player->AddHealth(crate->GetStrength() * 2.5f);
			} else if (IsKeyDown(KEY_E)) {
				player->AddAmmo(crate->GetStrength() * 2.5f);
			} else {
				player->AddAmmo(crate->GetStrength());
				player->AddHealth(crate->GetStrength());
			}
			continue;
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
		delete crate;
	}

	// The counter that determines death for 10 seconds
	deathTimer -= deltaTime;
	if (deathTimer <= 1.0f) {
		// Kill crates
		for (auto crate: crates) {
			delete crate;
		}
		crates.clear();
	}
	if (deathTimer <= 0.f) {
		deathTimer = DEATH_TIME;

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

		auto spawnAmounts = rando::GetRandomDistribution<3>(2.0 + floor((totalTime / 10.0)));
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
	player->GetBody()->ApplyInstantForce({0, 1, 0}, 15.0f);
}

void level::Level::SetEnemySpawn(glm::vec3 spawnLocation) {
	this->enemySpawns.push_back(spawnLocation);
}

void level::Level::SpawnEnemy(character::EnemyType enemyType, unsigned int numSpawns) {
	for (unsigned int i = 0; i < numSpawns; i++) {
		int spawnIdx = rando::GetRandomRange(0, (int)enemySpawns.size());
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
