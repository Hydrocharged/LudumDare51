// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <sound/manager.h>
#include <raylib.h>
#include <map>
#include <string>

#if defined(PLATFORM_WEB)
std::string pathPrefix = "";
#else
std::string pathPrefix = "../";
#endif // PLATFORM_WEB

std::map<sound::manager::Name, Sound> sounds;

void sound::manager::Load() {
	sounds[Name::GunShot] = LoadSound((pathPrefix + "assets/models/sounds/shooting.wav").c_str());
}