// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <render/model.h>
#include <physics/target.h>
#include <raymath.h>

void render::Model(::Model model, physics::Body* body, glm::vec3 scale) {
	Matrix matScale = MatrixScale(scale.x, scale.y, scale.z);
	glm::mat4 pMatRot = body->GetRotationMatrix();
	Matrix matRotation = Matrix{pMatRot[0][0], pMatRot[0][1], pMatRot[0][2], pMatRot[0][3],
								pMatRot[1][0], pMatRot[1][1], pMatRot[1][2], pMatRot[1][3],
								pMatRot[2][0], pMatRot[2][1], pMatRot[2][2], pMatRot[2][3],
								pMatRot[3][0], pMatRot[3][1], pMatRot[3][2], pMatRot[3][3]};
	glm::vec3 position = body->GetPosition();
	Matrix matTranslation = MatrixTranslate(position.x, position.y, position.z);
	Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
	model.transform = MatrixMultiply(model.transform, matTransform);
	for (int i = 0; i < model.meshCount; i++) {
		DrawMesh(model.meshes[i], model.materials[model.meshMaterial[i]], model.transform);
	}
}
