// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PHYSICS_COLLISION_H
#define PHYSICS_COLLISION_H
#include <glm/glm.hpp>

namespace physics {
	enum class ColliderType : unsigned int {
		Sphere = 1 << 0,
		Capsule = 1 << 1,
		AABB = 1 << 2,
		Lozenge = 1 << 3,
	};

	inline constexpr unsigned int operator+(ColliderType a, ColliderType b) {
		return static_cast<unsigned int>(static_cast<unsigned int>(a) + static_cast<unsigned int>(b));
	}

	typedef glm::vec3 Point;
	typedef glm::vec3 Vector;

	// A sphere with a single point.
	struct Sphere {
		Point c; // Center
		float r; // Radius
	};

	// A sphere swept from point A to B.
	struct Capsule {
		Point a; // Line segment start
		Point b; // Line segment end
		float r; // Radius
	};

	// A sphere swept across the rectangle U.
	struct Lozenge {
		Point a;     // Origin
		Vector u[2]; // The two edges axes of a rectangle
		float r;     // Radius
	};

	// region R = { (x, y, z) | min.x<=x<=max.x, min.y<=y<=max.y, min.z<=z<=max.z }
	struct AABB {
		Point min;
		Point max;
	};

	void ClosestPtPointSegment(Point c, Point a, Point b, float& t, Point& d);
	void ClosestPtPointAABB(physics::Point p, AABB b, Point& q);
	float ClosestPtSegmentSegment(Point p1, Point q1, Point p2, Point q2,
								  float& s, float& t, Point& c1, Point& c2);

	float SqDistPointSegment(Point a, Point b, Point c);

	bool TestRaySphere(Point p, Vector d, Sphere s);
	bool TestSphereCapsule(Sphere s, Capsule capsule);
	bool TestCapsuleCapsule(Capsule capsule1, Capsule capsule2);
	bool TestCapsuleAABB(Capsule capsule, AABB aabb);
	bool TestSphereSphere(Sphere a, Sphere b);
	bool TestSpherePoint(Sphere a, Point b);
	bool TestSphereAABB(Sphere sphere, AABB aabb);
	bool TestAABBAABB(AABB a, AABB b);

	glm::vec3 MoveSphereFromAABB(Sphere sphere, AABB aabb);
	glm::vec3 MoveCapsuleFromAABB(Capsule capsule, AABB aabb);
}

#endif //PHYSICS_COLLISION_H
