// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <model/manager.h>
#include <model/raylib.h>
#include <map>
#include <string>

#if defined(PLATFORM_WEB)
std::string pathPrefix = "";
#else
std::string pathPrefix = "../";
#endif // PLATFORM_WEB

std::map<model::manager::Name, Model> models;

void model::manager::Load() {
	Model model = LoadModel((pathPrefix + "assets/models/pistol/pistol.obj").c_str());
	Texture2D texture = LoadTexture((pathPrefix + "assets/models/pistol/pistol.png").c_str());
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
	models[Name::Pistol] = model;

	model = LoadModel((pathPrefix + "assets/models/shotgun/shotgun.obj").c_str());
	texture = LoadTexture((pathPrefix + "assets/models/shotgun/shotgun.png").c_str());
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
	models[Name::Shotgun] = model;

	model = LoadModel((pathPrefix + "assets/models/sniper/sniper.obj").c_str());
	texture = LoadTexture((pathPrefix + "assets/models/sniper/sniper.png").c_str());
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
	models[Name::Sniper] = model;

	model = LoadModel((pathPrefix + "assets/models/skull/skull.obj").c_str());
	texture = LoadTexture((pathPrefix + "assets/models/skull/skull.png").c_str());
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
	models[Name::Skull] = model;

	model = LoadModel((pathPrefix + "assets/models/turret/turret.obj").c_str());
	texture = LoadTexture((pathPrefix + "assets/models/turret/turret.png").c_str());
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
	models[Name::Turret] = model;

	model = LoadModel((pathPrefix + "assets/models/vampire/vampire.obj").c_str());
	texture = LoadTexture((pathPrefix + "assets/models/vampire/vampire.png").c_str());
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
	models[Name::Vampire] = model;

	model = LoadModel((pathPrefix + "assets/models/level1/level1.obj").c_str());
	texture = LoadTexture((pathPrefix + "assets/models/level1/level1.png").c_str());
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
	models[Name::Level1] = model;

	model = LoadModel((pathPrefix + "assets/models/crate/crate.obj").c_str());
	texture = LoadTexture((pathPrefix + "assets/models/crate/crate.png").c_str());
	SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);
	models[Name::Crate] = model;
}

void model::manager::Unload() {
	UnloadModel(models[Name::Pistol]);
	UnloadModel(models[Name::Shotgun]);
	UnloadModel(models[Name::Sniper]);
	UnloadModel(models[Name::Skull]);
	UnloadModel(models[Name::Turret]);
	UnloadModel(models[Name::Vampire]);
	UnloadModel(models[Name::Level1]);
}

Model model::manager::Get(Name name) {
	return models[name];
}
