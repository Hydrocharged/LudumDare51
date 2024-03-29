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
		glm::vec3 GetVelocity() { return velocity; }
		glm::vec3 GetHorizontalVelocity() { return glm::vec3{velocity.x, 0, velocity.z}; }
		glm::vec3 GetVerticalVelocity() { return glm::vec3{0, velocity.y, 0}; }
		glm::vec2 GetLookAngles() { return lookAngles; }
		float GetLookAngleX() { return lookAngles.x; }
		float GetLookAngleY() { return lookAngles.y; }
		void OffsetPosition(glm::vec3 amount) { position += amount; }
		void SetPosition(glm::vec3 pos) { this->position = pos; }
		void SetStationary(bool val) { stationary = val; }
		void SetGravity(bool enabled) { hasGravity = enabled; }
		void SetHorizontalDrag(float drag) { hDrag = drag; }
		void SetVerticalDrag(float drag) { vDrag = drag; }
		void SetLookAngleOffsets(glm::vec2 offsets) { lookAngleOffsets = offsets; }

		glm::vec3 GetPosition();
		glm::mat4 GetRotationMatrix();
		void SetParent(Body* p);
		void SetLookAngles(glm::vec2 angles);
		void Update(float deltaTime);
		void StopMomentum(glm::vec3 offsetAndDirection);
		void ApplyFrameForce(glm::vec3 direction, float magnitude);
		void ApplyInstantForce(glm::vec3 direction, float magnitude);
		void StopVelocity();
		bool CollidesWith(Body* otherBody);
		void LookAt(glm::vec3 target);

		virtual ColliderType ColliderType() = 0;
		virtual Sphere GetSphere() = 0;

	protected:
		Body* parent = nullptr;
		bool stationary = false;
		bool hasGravity = true;
		glm::vec3 position;
		glm::vec3 velocity = {};
		glm::vec3 frameAcceleration = {};
		glm::vec2 lookAngles = {};
		glm::vec2 lookAngleOffsets = {};
		float hDrag = 10.0f;
		float vDrag = 1.0f;
	};

	class SphereBody : public Body {
	public:
		SphereBody(glm::vec3 position, float radius) : Body(position), radius(radius) {}
		~SphereBody() override = default;
		enum ColliderType ColliderType() override { return ColliderType::Sphere; }
		operator Sphere() { return Sphere{GetPosition(), radius}; }
		Sphere GetSphere() override { return *this; }

	protected:
		float radius;
	};

	class CapsuleBody : public Body {
	public:
		CapsuleBody(glm::vec3 position, glm::vec3 a, glm::vec3 b, float radius) : Body(position), a(a), b(b), radius(radius), sphereRadius(glm::distance(a, b) + (radius * 2.0f)) {}
		~CapsuleBody() override = default;
		enum ColliderType ColliderType() override { return ColliderType::Capsule; }
		operator Capsule() { return Capsule{a + GetPosition(), b + GetPosition(), radius}; }
		Capsule GetCapsule() { return *this; }
		Sphere GetSphere() override { return Sphere{this->GetPosition(), sphereRadius}; }

	protected:
		glm::vec3 a;
		glm::vec3 b;
		float radius;
		float sphereRadius;
	};

	class AABBBody : public Body {
	public:
		AABBBody(glm::vec3 position, glm::vec3 whl) : Body(position), halfWHL(whl / 2.0f), sphereRadius(glm::length(halfWHL)) {}
		~AABBBody() override = default;
		enum ColliderType ColliderType() override { return ColliderType::AABB; }
		operator AABB() { return AABB{GetPosition() - halfWHL, GetPosition() + halfWHL}; }
		AABB GetAABB() { return *this; }
		Sphere GetSphere() override { return Sphere{this->GetPosition(), sphereRadius}; }
		[[nodiscard]] float Width() const { return halfWHL.x * 2.0f; }
		[[nodiscard]] float Height() const { return halfWHL.y * 2.0f; }
		[[nodiscard]] float Length() const { return halfWHL.z * 2.0f; }

	protected:
		glm::vec3 halfWHL; // Half width, height, length
		float sphereRadius;
	};
}

#endif //PHYSICS_BODY_H
