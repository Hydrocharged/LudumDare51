// Copyright © 2022 James Cor

#include <character/enemy.h>

character::Enemy::Enemy(glm::vec3 pos) {
	model = glm::translate(glm::mat4(1.0f),pos);
}