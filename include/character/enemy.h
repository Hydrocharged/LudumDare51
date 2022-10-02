// Copyright © 2022 James Cor

#ifndef CHARACTER_ENEMY_H
#define CHARACTER_ENEMY_H

#include <raylib.h>

namespace character {
	// TODO: polymorphism?
	class Enemy {
	public:
		Enemy(Vector3 pos);
		~Enemy() = default;

		void UpdatePosition(Vector3 playerPos); // input player location
		void Draw();

	private:
		Vector3 pos;
		float speed = 10.0f;
		// TODO: add enemy properties
		// speed, damage, etc
		// TODO: will need their own transformation matrix so they "look at" player
	};
}

#endif //CHARACTER_ENEMY_H
