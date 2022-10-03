// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <physics/body.h>

const float gravity = 9.8f;

void physics::Body::SetParent(physics::Body* p) {
	parent = p;
}

glm::vec3 physics::Body::Position() {
	if (parent == nullptr) {
		return position;
	}
	return position + parent->Position();
}

void physics::Body::Update(float deltaTime) {
	if (stationary || parent != nullptr) {
		return;
	}
	//TODO: disable movement if colliding with something
	if (hasGravity) {
		ApplyForce({0, -1.0f, 0}, gravity, deltaTime);
	}
	glm::vec3 horizontalComponents{velocity.x, 0, velocity.z};
	float horizontalMagnitude = glm::length(horizontalComponents);
	glm::vec3 verticalComponent{0, velocity.y, 0};
	float verticalMagnitude = velocity.y;
	if (horizontalMagnitude > FLT_EPSILON) {
		glm::vec3 newHComp = -(horizontalMagnitude * 4.0f * glm::log(horizontalMagnitude)) * (glm::normalize(horizontalComponents)) * deltaTime * hDrag;
		horizontalComponents += newHComp;
	}
	if (verticalMagnitude > FLT_EPSILON) {
		verticalComponent += -(verticalMagnitude * verticalMagnitude) * (glm::normalize(verticalComponent)) * deltaTime * vDrag;
	}
	velocity = horizontalComponents + verticalComponent;
	position += deltaTime * velocity;
}

void physics::Body::ApplyForce(glm::vec3 direction, float magnitude, float deltaTime) {
	// Can't apply forces on children, only the parent, and also the direction may not be zero
	if (stationary || parent != nullptr || glm::length(direction) < FLT_EPSILON) {
		return;
	}
	glm::vec3 acceleration = glm::normalize(direction) * magnitude;
	velocity += deltaTime * acceleration;
}

void physics::Body::ApplyInstantForce(glm::vec3 direction, float magnitude) {
	ApplyForce(direction, magnitude, 1.0f);
}

void physics::Body::StopVelocity() {
	velocity = glm::vec3(0);
}

bool handleSphereSphere(physics::SphereBody* aSphere, physics::SphereBody* bSphere) {
	return physics::TestSphereSphere(*aSphere, *bSphere);
}

bool handleSphereCapsule(physics::SphereBody* aSphere, physics::CapsuleBody* bCapsule) {
	return physics::TestSphereCapsule(*aSphere, *bCapsule);
}

bool handleSphereAABB(physics::SphereBody* aSphere, physics::AABBBody* bAABB) {
	physics::Sphere sphere = aSphere->GetSphere();
	physics::Point closestPoint = {};
	physics::ClosestPtPointAABB(sphere.c, *bAABB, closestPoint);
	return physics::TestSpherePoint(sphere, closestPoint);
}

bool handleCapsuleCapsule(physics::CapsuleBody* aCapsule, physics::CapsuleBody* bCapsule) {
	return physics::TestCapsuleCapsule(*aCapsule, *bCapsule);
}

bool handleCapsuleAABB(physics::CapsuleBody* aCapsule, physics::AABBBody* bAABB) {
	// We can do a quick comparison of the AABB's spherical bounds to see if we need to do a more in-depth check
	physics::Capsule capsule = aCapsule->GetCapsule();
	auto aabbSphere = physics::Sphere{bAABB->Position(), bAABB->SphereRadius()};
	if (!physics::TestSphereCapsule(aabbSphere, capsule)) {
		return false;
	}
	// We're doing 3 tests here on spheres to approximate a proper test of the entire capsule. This is definitely not
	// the best way to do this, but it works for now.
	physics::AABB aabb = bAABB->GetAABB();
	physics::Point closestPoint = {};
	// Check the first end cap
	physics::ClosestPtPointAABB(capsule.a, aabb, closestPoint);
	if (physics::TestSpherePoint(physics::Sphere{capsule.a, capsule.r}, closestPoint)) {
		return true;
	}
	// Check the other end cap
	physics::ClosestPtPointAABB(capsule.b, aabb, closestPoint);
	if (physics::TestSpherePoint(physics::Sphere{capsule.b, capsule.r}, closestPoint)) {
		return true;
	}
	// Check the middle
	physics::ClosestPtPointAABB(aCapsule->Position(), aabb, closestPoint);
	if (physics::TestSpherePoint(physics::Sphere{aCapsule->Position(), capsule.r}, closestPoint)) {
		return true;
	}
	// No collisions detected
	return false;
}

bool handleAABBAABB(physics::AABBBody* aAABB, physics::AABBBody* bAABB) {
	return physics::TestAABBAABB(*aAABB, *bAABB);
}

bool physics::Body::CollidesWith(physics::Body* otherBody) {
	enum ColliderType thisCollider = this->ColliderType();
	enum ColliderType otherCollider = otherBody->ColliderType();
	switch (thisCollider + otherCollider) {
		case ColliderType::Sphere + ColliderType::Sphere: {
			return handleSphereSphere(static_cast<physics::SphereBody*>(this), static_cast<physics::SphereBody*>(otherBody));
		}
		case ColliderType::Sphere + ColliderType::Capsule: {
			if (thisCollider == ColliderType::Sphere) {
				return handleSphereCapsule(static_cast<physics::SphereBody*>(this), static_cast<physics::CapsuleBody*>(otherBody));
			} else {
				return handleSphereCapsule(static_cast<physics::SphereBody*>(otherBody), static_cast<physics::CapsuleBody*>(this));
			}
		}
		case ColliderType::Sphere + ColliderType::AABB: {
			if (thisCollider == ColliderType::Sphere) {
				return handleSphereAABB(static_cast<physics::SphereBody*>(this), static_cast<physics::AABBBody*>(otherBody));
			} else {
				return handleSphereAABB(static_cast<physics::SphereBody*>(otherBody), static_cast<physics::AABBBody*>(this));
			}
		}
		case ColliderType::Capsule + ColliderType::Capsule: {
			return handleCapsuleCapsule(static_cast<physics::CapsuleBody*>(this), static_cast<physics::CapsuleBody*>(otherBody));
		}
		case ColliderType::Capsule + ColliderType::AABB: {
			if (thisCollider == ColliderType::Capsule) {
				return handleCapsuleAABB(static_cast<physics::CapsuleBody*>(this), static_cast<physics::AABBBody*>(otherBody));
			} else {
				return handleCapsuleAABB(static_cast<physics::CapsuleBody*>(otherBody), static_cast<physics::AABBBody*>(this));
			}
		}
		case ColliderType::AABB + ColliderType::AABB: {
			return handleAABBAABB(static_cast<physics::AABBBody*>(this), static_cast<physics::AABBBody*>(otherBody));
		}
	}
	return false;
}
