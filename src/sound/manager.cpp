// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <map>
#include <string>
#include <sound/manager.h>
#include <sound/raylib.h>

#if defined(PLATFORM_WEB)
std::string soundAssetPrefix = "";
#else
std::string soundAssetPrefix = "../";
#endif // PLATFORM_WEB

std::map<sound::manager::Name, Sound> sounds;

void sound::manager::Load() {
	sounds[Name::Pistol] = LoadSound((soundAssetPrefix + "assets/sounds/pistol.wav").c_str());
	sounds[Name::Shotgun] = LoadSound((soundAssetPrefix + "assets/sounds/shotgun.wav").c_str());
	sounds[Name::Sniper] = LoadSound((soundAssetPrefix + "assets/sounds/sniper.wav").c_str());
	sounds[Name::Ouch] = LoadSound((soundAssetPrefix + "assets/sounds/ouch.wav").c_str());
}

void sound::manager::Unload() {
	UnloadSound(sounds[Name::Shotgun]);
}

Sound sound::manager::Get(sound::manager::Name name) {
	return sounds[name];
}