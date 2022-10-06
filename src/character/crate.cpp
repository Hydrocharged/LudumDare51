// Copyright © 2022 James Cor
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <character/crate.h>
#include <render/model.h>

character::Crate::Crate(float strength, glm::vec3 pos) {
	this->strength = strength;
	model = model::manager::Get(model::manager::Name::Crate);
	body = std::make_unique<physics::SphereBody>(pos, 1.0f);
}

void character::Crate::Update(float deltaTime) {
	spin += 1.5f * deltaTime;
	if (spin > 2 * PI) {
		spin -= 2 * PI;
	}
	body->SetLookAngles({spin, 0.f});
	body->Update(deltaTime);
}

void character::Crate::Draw(float deltaTime) {
	glm::vec3 pos = body->GetPosition();
	render::Model(model, body.get(), {0, (glm::cos(spin) * 0.2f) + 0.1f, 0}, glm::vec3(0.4f));
}
