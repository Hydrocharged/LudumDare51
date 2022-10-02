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
		Plane = 1 << 0,
		Sphere = 1 << 1,
		Capsule = 1 << 2,
		Lozenge = 1 << 3,
		Rect = 1 << 4,
		AABB = 1 << 5,
	};

	inline constexpr unsigned int operator+(ColliderType a, ColliderType b) {
		return static_cast<unsigned int>(static_cast<unsigned int>(a) + static_cast<unsigned int>(b));
	}

	typedef glm::vec3 Point;
	typedef glm::vec3 Vector;

	struct Plane {
		Vector n; // Plane normal. Points x on the plane satisfy Dot(n,x) = d
		float d; // d = dot(n,p) for a given point p on the plane
	};

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

	struct Rect {
		Point c; // center point of rectangle
		Vector u[2]; // unit vectors determining local x and y axes for the rectangle
		float e[2]; // the halfwidth extents of the rectangle along the axes
	};

	// region R = { (x, y, z) | min.x<=x<=max.x, min.y<=y<=max.y, min.z<=z<=max.z }
	struct AABB {
		Point min;
		Point max;
	};

	float DistPointPlane(Point q, Plane p);

	Point ClosestPtPointPlane(Point q, Plane p);
	void ClosestPtPointSegment(Point c, Point a, Point b, float& t, Point& d);
	void ClosestPtPointRect(Point p, Rect r, Point& q);
	void ClosestPtPointAABB(physics::Point p, physics::AABB b, physics::Point& q);
	float ClosestPtSegmentSegment(Point p1, Point q1, Point p2, Point q2,
								  float& s, float& t, Point& c1, Point& c2);

	float SqDistPointSegment(Point a, Point b, Point c);

	bool TestSegmentPlane(Point a, Point b, Plane p, float& t, Point& q);
	bool TestRaySphere(Point p, Vector d, Sphere s);
	bool TestSphereCapsule(Sphere s, Capsule capsule);
	bool TestCapsuleCapsule(Capsule capsule1, Capsule capsule2);
	bool TestMovingSpherePlane(Point a, Point b, float r, Plane p);
	bool TestSphereSphere(Sphere a, Sphere b);
	bool TestSpherePoint(physics::Sphere a, physics::Point b);
	bool TestAABBAABB(physics::AABB a, physics::AABB b);
}

#endif //PHYSICS_COLLISION_H
