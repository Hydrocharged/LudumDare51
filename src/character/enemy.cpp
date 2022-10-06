// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/enemy.h>
#include <string>

#if defined(PLATFORM_WEB)
std::string enemyTexturePrefix = "";
#else
std::string enemyTexturePrefix = "../";
#endif // PLATFORM_WEB

bool redTextureInitialized = false;
Texture2D redTexture;

character::Enemy::Enemy(physics::CapsuleBody* body, Model model) {
	if(!redTextureInitialized) {
		redTexture = LoadTexture((enemyTexturePrefix + "assets/textures/red.png").c_str());
	}
	this->body = body;
	this->model = model;
	for (int i = 0; i < model.meshCount; i++) {
		tints.push_back(model.materials[model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].color);
		textures.push_back(model.materials[model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].texture);
	}
}

void character::Enemy::TintModel() {
	if(health <= 0.0f) {
		for (int i = 0; i < model.meshCount; i++) {
			model.materials[model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].texture = redTexture;
		}
	} else {
		auto amount = (unsigned char)(255.0f * (health / maxHealth));
		Color tint = WHITE;
		tint.g = amount;
		tint.b = amount;
		for (int i = 0; i < model.meshCount; i++) {
			model.materials[model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].color = tint;
		}
	}
}

void character::Enemy::UntintModel() {
	if (health <= 0.0f) {
		for (int i = 0; i < model.meshCount; i++) {
			model.materials[model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].texture = textures[i];
		}
	} else {
		for (int i = 0; i < model.meshCount; i++) {
			model.materials[model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].color = tints[i];
		}
	}
}
