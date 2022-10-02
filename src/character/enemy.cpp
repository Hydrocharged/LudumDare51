// Copyright © 2022 James Cor

#include <character/enemy.h>


character::Enemy::Enemy(Vector3 pos) {
	this->pos = pos;
}

void character::Enemy::UpdatePosition(Vector3 playerPos) {
	// TODO: use glm minus and normalize
	Vector3 dir = {playerPos.x - pos.x, playerPos.y - pos.y, playerPos.z - pos.z};
	float tmp = GetFrameTime() * speed;
	pos = {dir.x * tmp, dir.y * tmp, dir.z * tmp};
}
void character::Enemy::Draw() {
	DrawCube(pos, 2, 2, 2, RED);
}