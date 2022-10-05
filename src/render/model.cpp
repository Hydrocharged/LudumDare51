// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <render/model.h>
#include <physics/target.h>
#include <raymath.h>

void render::Model(::Model model, physics::Body* body) {
	Model(model, body->GetPosition(), body->GetRotationMatrix(), glm::vec3(1.0f));
}

void render::Model(::Model model, physics::Body* body, glm::vec3 scale) {
	Model(model, body->GetPosition(), body->GetRotationMatrix(), scale);
}

void render::Model(::Model model, physics::Body* body, glm::mat4 rotMatrix) {
	Model(model, body->GetPosition(), rotMatrix, glm::vec3(1.0f));
}

void render::Model(::Model model, physics::Body* body, glm::mat4 rotMatrix, glm::vec3 scale) {
	Model(model, body->GetPosition(), rotMatrix, scale);
}

void render::Model(::Model model, physics::Body* body, glm::vec3 renderPosOffset, glm::vec3 scale) {
	Model(model, body->GetPosition() + renderPosOffset, body->GetRotationMatrix(), scale);
}

void render::Model(::Model model, glm::vec3 position, glm::mat4 rotMatrix) {
	Model(model, position, rotMatrix, glm::vec3(1.0f));
}

void render::Model(::Model model, glm::vec3 position, glm::mat4 rotMatrix, glm::vec3 scale) {
	Matrix matScale = MatrixScale(scale.x, scale.y, scale.z);
	Matrix matRotation = Matrix{rotMatrix[0][0], rotMatrix[0][1], rotMatrix[0][2], rotMatrix[0][3],
								rotMatrix[1][0], rotMatrix[1][1], rotMatrix[1][2], rotMatrix[1][3],
								rotMatrix[2][0], rotMatrix[2][1], rotMatrix[2][2], rotMatrix[2][3],
								rotMatrix[3][0], rotMatrix[3][1], rotMatrix[3][2], rotMatrix[3][3]};
	Matrix matTranslation = MatrixTranslate(position.x, position.y, position.z);
	Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
	model.transform = MatrixMultiply(model.transform, matTransform);
	for (int i = 0; i < model.meshCount; i++) {
		DrawMesh(model.meshes[i], model.materials[model.meshMaterial[i]], model.transform);
	}
}
