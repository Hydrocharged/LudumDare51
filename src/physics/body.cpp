// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <physics/body.h>

const float gravity = 30.0f;

void physics::Body::SetParent(physics::Body* p) {
	parent = p;
}

glm::vec3 physics::Body::GetPosition() {
	if (parent == nullptr) {
		return position;
	}
	return position + parent->GetPosition();
}

void physics::Body::Update(float deltaTime) {
	if (stationary || parent != nullptr) {
		return;
	} else if (hasGravity) {
		ApplyFrameForce({0, -1.0f, 0}, gravity);
	}

	glm::vec3 hComp{velocity.x, 0, velocity.z};
	float hMag = glm::length(hComp);
	glm::vec3 vComp{0, velocity.y, 0};
	float vMag = velocity.y;
	if (hMag > FLT_EPSILON) {
		frameAcceleration -= (hMag * hDrag) * (glm::normalize(hComp));
	}
	if (vMag > FLT_EPSILON) {
		frameAcceleration -= (vMag * vDrag) * (glm::normalize(vComp));
	}
	position += (velocity * deltaTime) + (frameAcceleration * deltaTime * deltaTime * 0.5f);
	velocity += frameAcceleration * deltaTime;
	frameAcceleration = {};
}

void physics::Body::StopMomentum(glm::vec3 offsetAndDirection) {
	position += offsetAndDirection;
	float velocityMagnitude = glm::length(velocity);
	if (velocityMagnitude <= FLT_EPSILON) {
		return;
	}
	glm::vec3 normalizedVelocity = glm::normalize(velocity);
	glm::vec3 normalizedFlippedDirection = glm::normalize(-offsetAndDirection);
	glm::vec3 counteractVelocity = glm::vec3{
		abs(velocity.x) * normalizedFlippedDirection.x,
		abs(velocity.y) * normalizedFlippedDirection.y,
		abs(velocity.z) * normalizedFlippedDirection.z,
	};
	velocity -= counteractVelocity;
}

void physics::Body::ApplyFrameForce(glm::vec3 direction, float magnitude) {
	// Can't apply forces on children, only the parent, and also the direction may not be zero
	if (stationary || parent != nullptr || glm::length(direction) < FLT_EPSILON) {
		return;
	}
	frameAcceleration += glm::normalize(direction) * magnitude;
}

void physics::Body::ApplyInstantForce(glm::vec3 direction, float magnitude) {
	// Can't apply forces on children, only the parent, and also the direction may not be zero
	if (stationary || parent != nullptr || glm::length(direction) < FLT_EPSILON) {
		return;
	}
	velocity += glm::normalize(direction) * magnitude;
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
	return physics::TestSphereAABB(*aSphere, *bAABB);
}

bool handleCapsuleCapsule(physics::CapsuleBody* aCapsule, physics::CapsuleBody* bCapsule) {
	return physics::TestCapsuleCapsule(*aCapsule, *bCapsule);
}

bool handleCapsuleAABB(physics::CapsuleBody* aCapsule, physics::AABBBody* bAABB) {
	return physics::TestCapsuleAABB(*aCapsule, *bAABB);
}

bool handleAABBAABB(physics::AABBBody* aAABB, physics::AABBBody* bAABB) {
	return physics::TestAABBAABB(*aAABB, *bAABB);
}

bool physics::Body::CollidesWith(physics::Body* otherBody) {
	// Do a quick sphere check. If they do not touch, there's no need to do the more expensive & accurate checks.
	if (!physics::TestSphereSphere(this->GetSphere(), otherBody->GetSphere())) {
		return false;
	}

	enum ColliderType thisCollider = this->ColliderType();
	enum ColliderType otherCollider = otherBody->ColliderType();
	switch (thisCollider + otherCollider) {
		case ColliderType::Sphere + ColliderType::Sphere: {
			// This is covered in the quick case before this switch
			return true;
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
