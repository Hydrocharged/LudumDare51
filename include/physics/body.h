// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H
#include <physics/collision.h>
#include <vector>

namespace physics {
	class Body {
	public:
		explicit Body(glm::vec3 position) : position(position) {}
		virtual ~Body() = default;
		void SetPosition(glm::vec3 pos) { this->position = pos; }
		void SetStationary(bool val) { stationary = val; }

		glm::vec3 Position();
		void SetParent(Body* p);
		void ApplyForce(glm::vec3 direction, float magnitude);
		bool CollidesWith(Body* otherBody);

		virtual ColliderType ColliderType() = 0;

	protected:
		Body* parent = nullptr;
		bool stationary = false;
		glm::vec3 position;
		glm::vec3 velocity = {};
		float maxHorizontalMagnitude = 1.0f;
		float maxVerticalMagnitude = 1.0f;
	};

	class SphereBody : public Body {
	public:
		SphereBody(glm::vec3 position, float radius) : Body(position), radius(radius) {}
		~SphereBody() override = default;
		enum ColliderType ColliderType() override { return ColliderType::Sphere; }
		operator Sphere() { return Sphere{Position(), radius}; }

	protected:
		float radius;
	};

	class CapsuleBody : public Body {
	public:
		CapsuleBody(glm::vec3 position, glm::vec3 a, glm::vec3 b, float radius) : Body(position), a(a), b(b), radius(radius) {}
		~CapsuleBody() override = default;
		enum ColliderType ColliderType() override { return ColliderType::Capsule; }
		operator Capsule() { return Capsule{a + Position(), b + Position(), radius}; }

	protected:
		glm::vec3 a;
		glm::vec3 b;
		float radius;
	};

	class AABBBody : public Body {
	public:
		AABBBody(glm::vec3 position, glm::vec3 whl) : Body(position), halfWHL(whl / 2.0f), sphereRadius(glm::length(halfWHL)) {}
		~AABBBody() override = default;
		enum ColliderType ColliderType() override { return ColliderType::AABB; }
		operator AABB() { return AABB{Position() - halfWHL, Position() + halfWHL}; }
		[[nodiscard]] float SphereRadius() const { return sphereRadius; }

	protected:
		glm::vec3 halfWHL; // Half width, height, length
		float sphereRadius;
	};
}

#endif //PHYSICS_BODY_H
