// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <stats/frame.h>
#include <raylib.h>

stats::Frame currentFrame{};

void stats::Frame::StartFrame() {
	currentFrame.PlayerCollisionChecks = 0;
	currentFrame.EnemyCollisionChecks = 0;
	currentFrame.ItemCollisionChecks = 0;
	currentFrame.ProjectileCollisionChecks = 0;
	currentFrame.LevelCollisionChecks = 0;

	currentFrame.PlayerCollisions = 0;
	currentFrame.EnemyCollisions = 0;
	currentFrame.ItemCollisions = 0;
	currentFrame.ProjectileCollisions = 0;
	currentFrame.LevelCollisions = 0;

	currentFrame.frameStart = GetTime();
}

void stats::Frame::EndFrame() {
	currentFrame.TotalFrametimeMs = (float)((GetTime() - currentFrame.frameStart) * 1000.0);
}

stats::Frame* stats::Frame::Current() {
	return &currentFrame;
}

std::string stats::Frame::CollisionChecksString() {
	return "Player: " + std::to_string(PlayerCollisionChecks) +
		   ", Enemy: " + std::to_string(EnemyCollisionChecks) +
		   ", Item: " + std::to_string(ItemCollisionChecks) +
		   ", Projectile: " + std::to_string(ProjectileCollisionChecks) +
		   ", Level: " + std::to_string(LevelCollisionChecks);
}

std::string stats::Frame::CollisionsString() {
	return "Player: " + std::to_string(PlayerCollisions) +
		   ", Enemy: " + std::to_string(EnemyCollisions) +
		   ", Item: " + std::to_string(ItemCollisions) +
		   ", Projectile: " + std::to_string(ProjectileCollisions) +
		   ", Level: " + std::to_string(LevelCollisions);
}

std::string stats::Frame::TotalFrametimeString() {
	return "Frametime: " + std::to_string(TotalFrametimeMs) + "ms";
}
