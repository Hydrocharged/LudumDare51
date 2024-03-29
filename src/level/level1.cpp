// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <level/level.h>
#include <random.h>

std::unique_ptr<level::Level> level::GetLevel1() {
	level::Level* level = new level::Level();
	level->SetDimensions({64, 64, 64});
	level->SetEnemySpawn({0, 3, 10});
	level->SetEnemySpawn({0, 3, -10});
	level->SetEnemySpawn({10, 3, 0});
	level->SetEnemySpawn({10, 3, 10});
	level->SetEnemySpawn({10, 3, -10});
	level->SetEnemySpawn({-10, 3, 0});
	level->SetEnemySpawn({-10, 3, 10});
	level->SetEnemySpawn({-10, 3, -10});
	level->SetEnemySpawn({20, 3, 10});
	level->SetEnemySpawn({20, 3, -10});
	level->SetEnemySpawn({10, 3, 20});
	level->SetEnemySpawn({10, 3, -20});
	level->SetEnemySpawn({-20, 3, 10});
	level->SetEnemySpawn({-20, 3, -10});
	level->SetEnemySpawn({-10, 3, 20});
	level->SetEnemySpawn({-10, 3, -20});
	level->SetEnemySpawn({0, 3, 20});
	level->SetEnemySpawn({0, 3, -20});
	level->SetEnemySpawn({20, 3, 0});
	level->SetEnemySpawn({20, 3, 20});
	level->SetEnemySpawn({20, 3, -20});
	level->SetEnemySpawn({-20, 3, 0});
	level->SetEnemySpawn({-20, 3, 20});
	level->SetEnemySpawn({-20, 3, -20});
	level->SetEnemySpawn({0, 3, 30});
	level->SetEnemySpawn({0, 3, -30});
	level->SetEnemySpawn({30, 3, 0});
	level->SetEnemySpawn({30, 3, 10});
	level->SetEnemySpawn({30, 3, -10});
	level->SetEnemySpawn({10, 3, 30});
	level->SetEnemySpawn({10, 3, -30});
	level->SetEnemySpawn({-30, 3, 0});
	level->SetEnemySpawn({-30, 3, 10});
	level->SetEnemySpawn({-30, 3, -10});
	level->SetEnemySpawn({-10, 3, 30});
	level->SetEnemySpawn({-10, 3, -30});
	level->SetEnemySpawn({20, 3, 30});
	level->SetEnemySpawn({20, 3, -30});
	level->SetEnemySpawn({30, 3, 20});
	level->SetEnemySpawn({30, 3, -20});
	level->SetEnemySpawn({-20, 3, 30});
	level->SetEnemySpawn({-20, 3, -30});
	level->SetEnemySpawn({-30, 3, 20});
	level->SetEnemySpawn({-30, 3, -20});
	level->SetEnemySpawn({30, 3, 30});
	level->SetEnemySpawn({30, 3, -30});
	level->SetEnemySpawn({-30, 3, 30});
	level->SetEnemySpawn({-30, 3, -30});

	level->AddBody(new physics::AABBBody(glm::vec3{0, -0.5, 0}, glm::vec3{32, 1, 32}));

	level->AddBody(new physics::AABBBody(glm::vec3{0, -0.5, 20}, glm::vec3{32, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{0, -0.5, -20}, glm::vec3{32, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{20, -0.5, 0}, glm::vec3{8, 1, 32}));
	level->AddBody(new physics::AABBBody(glm::vec3{-20, -0.5, 0}, glm::vec3{8, 1, 32}));

	level->AddBody(new physics::AABBBody(glm::vec3{0, 0.5, 28}, glm::vec3{48, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{0, 0.5, -28}, glm::vec3{48, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{28, 0.5, 0}, glm::vec3{8, 1, 48}));
	level->AddBody(new physics::AABBBody(glm::vec3{-28, 0.5, 0}, glm::vec3{8, 1, 48}));

	level->AddBody(new physics::AABBBody(glm::vec3{20, 0.5, 20}, glm::vec3{8, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{20, 0.5, -20}, glm::vec3{8, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{-20, 0.5, 20}, glm::vec3{8, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{-20, 0.5, -20}, glm::vec3{8, 1, 8}));

	level->AddBody(new physics::AABBBody(glm::vec3{28, 1.5, 28}, glm::vec3{8, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{28, 1.5, -28}, glm::vec3{8, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{-28, 1.5, 28}, glm::vec3{8, 1, 8}));
	level->AddBody(new physics::AABBBody(glm::vec3{-28, 1.5, -28}, glm::vec3{8, 1, 8}));

	level->AddBody(new physics::AABBBody(glm::vec3{33, 12.5, 0}, glm::vec3{2, 25, 64}));
	level->AddBody(new physics::AABBBody(glm::vec3{-33, 12.5, 0}, glm::vec3{2, 25, 64}));
	level->AddBody(new physics::AABBBody(glm::vec3{0, 12.5, 33}, glm::vec3{64, 25, 2}));
	level->AddBody(new physics::AABBBody(glm::vec3{0, 12.5, -33}, glm::vec3{64, 25, 2}));

	level->SetPlayerSpawn({0, 5, 0});
	level->SpawnPlayer();
	level->SpawnEnemy(character::EnemyType::Skull, 1);

	return std::unique_ptr<level::Level>(level);
}
