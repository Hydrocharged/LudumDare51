// Copyright © 2022 James Cor & Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef STATS_FRAME_H
#define STATS_FRAME_H
#include <string>

namespace stats {
	struct Frame {
		int PlayerCollisionChecks = 0;
		int EnemyCollisionChecks = 0;
		int ItemCollisionChecks = 0;
		int ProjectileCollisionChecks = 0;
		int LevelCollisionChecks = 0;

		int PlayerCollisions = 0;
		int EnemyCollisions = 0;
		int ItemCollisions = 0;
		int ProjectileCollisions = 0;
		int LevelCollisions = 0;

		float TotalFrametimeMs = 0.0f;

		static void StartFrame();
		static void EndFrame();
		static Frame* Current();
		std::string CollisionChecksString();
		std::string CollisionsString();
		std::string TotalFrametimeString();

	private:
		double frameStart = 0.0;
	};
}

#endif //STATS_FRAME_H
