// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MODEL_RAYLIB_H
#define MODEL_RAYLIB_H
#include <raylib.h>

namespace model::manager {
	enum class Name {
		Pistol,
		Shotgun,
		Sniper,
		Skull,
		Turret,
		Vampire,
		Level1,
		Crate,
		Bullet,
		EnemyProjectile,
	};
	Model Get(Name name);
}

#endif //MODEL_RAYLIB_H
