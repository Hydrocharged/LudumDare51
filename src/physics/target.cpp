// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <physics/target.h>

const float pi = 3.14159265358979323846f;
const float angleYMin = -89.0f * (pi / 180.0f);
const float angleYMax = 89.0f * (pi / 180.0f);

glm::vec3 physics::GetTargetVector(glm::vec3 position, float angleX, float angleY) {
	glm::mat4 matTranslation(1.0f);
	glm::mat4 matRotation = GetRotationMatrix(angleX, angleY);
	matTranslation[2][3] = 10.0f;

	glm::mat4 matTransform = matTranslation * matRotation;
	return glm::vec3{
		position.x - matTransform[0][3],
		position.y - matTransform[1][3],
		position.z - matTransform[2][3]
	};
}

void physics::GetAngleXY(glm::vec3 position, glm::vec3 target, glm::vec2& angles) {
	GetAngleXY(position, target, angles.x, angles.y);
}

void physics::GetAngleXY(glm::vec3 position, glm::vec3 target, float& angleX, float& angleY) {
	glm::vec3 direction = target - position;
	angleX = atan2f(-direction.x, -direction.z);
	angleY = atan2f(direction.y, sqrtf(direction.x * direction.x + direction.z * direction.z));
}

void physics::SanitizeAngles(glm::vec2& angles) {
	SanitizeAngles(angles.x, angles.y);
}

void physics::SanitizeAngles(float& angleX, float& angleY) {
	if (angleY > angleYMax) {
		angleY = angleYMax;
	} else if (angleY < angleYMin) {
		angleY = angleYMin;
	}
	angleX = glm::mod(angleX, 2 * pi);
}

glm::mat4 physics::GetRotationMatrix(glm::vec2 angles) {
	return GetRotationMatrix(angles.x, angles.y);
}

glm::mat4 physics::GetRotationMatrix(float angleX, float angleY) {
	glm::mat4 matRotation(1.0f);

	float cosz = cosf(0.0f);
	float sinz = sinf(0.0f);
	float cosy = cosf(-(pi * 2 - angleX));
	float siny = sinf(-(pi * 2 - angleX));
	float cosx = cosf(-(pi * 2 - angleY));
	float sinx = sinf(-(pi * 2 - angleY));

	matRotation[0][0] = cosz * cosy;
	matRotation[0][1] = (cosz * siny * sinx) - (sinz * cosx);
	matRotation[0][2] = (cosz * siny * cosx) + (sinz * sinx);
	matRotation[1][0] = sinz * cosy;
	matRotation[1][1] = (sinz * siny * sinx) + (cosz * cosx);
	matRotation[1][2] = (sinz * siny * cosx) - (cosz * sinx);
	matRotation[2][0] = -siny;
	matRotation[2][1] = cosy * sinx;
	matRotation[2][2] = cosy * cosx;

	return matRotation;
}
