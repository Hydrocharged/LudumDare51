// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SOUND_RAYLIB_H
#define SOUND_RAYLIB_H
#include <raylib.h>

namespace sound::manager {
	enum class Name {
		Pistol,
		Shotgun,
		Sniper,
		Ouch,
	};
	Sound Get(Name name);
}

#endif //SOUND_RAYLIB_H
