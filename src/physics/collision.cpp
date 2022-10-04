// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <physics/collision.h>

// clamp n to lie within the range [min, max]
float clamp(float n, float min, float max) {
	if (n < min) { return min; }
	if (n > max) { return max; }
	return n;
}

// Given segment ab and point c, computes closest point d on ab.
// Also returns t for the position of d, d(t) = a + t*(b - a)
void physics::ClosestPtPointSegment(physics::Point c, physics::Point a, physics::Point b, float& t, physics::Point& d) {
	physics::Vector ab = b - a;
	// Project c onto ab, computing parameterized position d(t) = a + t*(b - a)
	t = glm::dot(c - a, ab) / glm::dot(ab, ab);
	// If outside segment, clamp t (and therefore d) to the closest endpoint
	if (t < 0.0f) { t = 0.0f; }
	if (t > 1.0f) { t = 1.0f; }
	// Compute projected position from the clamped t
	d = a + t * ab;
}

// Given point p, return the point q on or in AABB b that is closest to p
void physics::ClosestPtPointAABB(physics::Point p, physics::AABB b, physics::Point& q) {
	// For each coordinate axis, if the point coordinate value is
	// outside box, clamp it to the box, else keep it as is
	for (int i = 0; i < 3; i++) {
		float v = p[i];
		if (v < b.min[i]) { v = b.min[i]; } // v = max(v, b.min[i])
		if (v > b.max[i]) { v = b.max[i]; } // v = min(v, b.max[i])
		q[i] = v;
	}
}

// Computes the closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
// distance between S1(s) and S2(t)
float physics::ClosestPtSegmentSegment(physics::Point p1, physics::Point q1, physics::Point p2, physics::Point q2,
									   float& s, float& t, physics::Point& c1, physics::Point& c2) {
	physics::Vector d1 = q1 - p1; // Direction vector of segment S1
	physics::Vector d2 = q2 - p2; // Direction vector of segment S2
	physics::Vector r = p1 - p2;
	float a = glm::dot(d1, d1); // Squared length of segment S1, always nonnegative
	float e = glm::dot(d2, d2); // Squared length of segment S2, always nonnegative
	float f = glm::dot(d2, r);
	// Check if either or both segments degenerate into points
	if (a <= FLT_EPSILON && e <= FLT_EPSILON) {
		// Both segments degenerate into points
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		return glm::dot(c1 - c2, c1 - c2);
	}
	if (a <= FLT_EPSILON) {
		// First segment degenerates into a point
		s = 0.0f;
		t = f / e; // s = 0 => t = (b*s + f) / e = f / e
		t = clamp(t, 0.0f, 1.0f);
	} else {
		float c = glm::dot(d1, r);
		if (e <= FLT_EPSILON) {
			// Second segment degenerates into a point
			t = 0.0f;
			s = clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
		} else {
			// The general nondegenerate case starts here
			float b = glm::dot(d1, d2);
			float denom = a * e - b * b; // Always nonnegative
			// If segments not parallel, compute closest point on L1 to L2 and
			// clamp to segment S1. Else pick arbitrary s (here 0)
			if (denom != 0.0f) {
				s = clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			} else { s = 0.0f; }
			// Compute point on L2 closest to S1(s) using
			// t = glm::dot((P1 + D1*s) - P2,D2) / glm::dot(D2,D2) = (b*s + f) / e
			t = (b * s + f) / e;
			// If t in [0,1] done. Else clamp t, recompute s for the new value
			// of t using s = glm::dot((P2 + D2*t) - P1,D1) / glm::dot(D1,D1)= (t*b - c) / a
			// and clamp s to [0, 1]
			if (t < 0.0f) {
				t = 0.0f;
				s = clamp(-c / a, 0.0f, 1.0f);
			} else if (t > 1.0f) {
				t = 1.0f;
				s = clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}
	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
	return glm::dot(c1 - c2, c1 - c2);
}

// Returns the squared distance between point c and segment ab
float physics::SqDistPointSegment(physics::Point a, physics::Point b, physics::Point c) {
	physics::Vector ab = b - a, ac = c - a, bc = c - b;
	float e = glm::dot(ac, ab);
	// Handle cases where c projects outside ab
	if (e <= 0.0f) { return glm::dot(ac, ac); }
	float f = glm::dot(ab, ab);
	if (e >= f) { return glm::dot(bc, bc); }
	// Handle cases where c projects onto ab
	return glm::dot(ac, ac) - e * e / f;
}

// Test if ray r = p + td intersects sphere s
bool physics::TestRaySphere(physics::Point p, physics::Vector d, physics::Sphere s) {
	physics::Vector m = p - s.c;
	float c = glm::dot(m, m) - s.r * s.r;
	// If there is definitely at least one real root, there must be an intersection
	if (c <= 0.0f) { return true; }
	float b = glm::dot(m, d);
	// Early exit if ray origin outside sphere and ray pointing away from sphere
	if (b > 0.0f) { return false; }
	float disc = b * b - c;
	// A negative discriminant corresponds to ray missing sphere
	if (disc < 0.0f) { return false; }
	// Now ray must hit sphere
	return true;
}

bool physics::TestSphereCapsule(physics::Sphere s, physics::Capsule capsule) {
	// Compute (squared) distance between sphere center and capsule line segment
	float dist2 = SqDistPointSegment(capsule.a, capsule.b, s.c);
	// If (squared) distance smaller than (squared) sum of radii, they collide
	float radius = s.r + capsule.r;
	return dist2 <= radius * radius;
}

bool physics::TestCapsuleCapsule(physics::Capsule capsule1, physics::Capsule capsule2) {
	// Compute (squared) distance between the inner structures of the capsules
	float s, t;
	physics::Point c1, c2;
	float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b,
		capsule2.a, capsule2.b, s, t, c1, c2);
	// If (squared) distance smaller than (squared) sum of radii, they collide
	float radius = capsule1.r + capsule2.r;
	return dist2 <= radius * radius;
}

bool physics::TestCapsuleAABB(Capsule capsule, AABB aabb) {
	// We're doing 3 tests here on spheres to approximate a proper test of the entire capsule. This is definitely not
	// the best way to do this, but it works for now.

	// Check the first end cap
	if (physics::TestSphereAABB(physics::Sphere{capsule.a, capsule.r}, aabb)) {
		return true;
	}
	// Check the other end cap
	if (physics::TestSphereAABB(physics::Sphere{capsule.b, capsule.r}, aabb)) {
		return true;
	}
	// Check the middle
	if (physics::TestSphereAABB(physics::Sphere{capsule.a + ((capsule.b - capsule.a) / 2.0f), capsule.r}, aabb)) {
		return true;
	}
	// No collisions detected
	return false;
}

bool physics::TestSphereSphere(physics::Sphere a, physics::Sphere b) {
	// Calculate squared distance between centers
	physics::Vector d = a.c - b.c;
	float dist2 = glm::dot(d, d);
	// Spheres intersect if squared distance is less than squared sum of radii
	float radiusSum = a.r + b.r;
	return dist2 <= radiusSum * radiusSum;
}

bool physics::TestSpherePoint(physics::Sphere a, physics::Point b) {
	// Calculate squared distance between center and point
	physics::Vector d = a.c - b;
	float dist2 = glm::dot(d, d);
	// Point intersects if squared distance is less than squared the radius
	return dist2 <= a.r * a.r;
}

bool physics::TestSphereAABB(physics::Sphere sphere, physics::AABB aabb) {
	if (aabb.min.x < sphere.c.x && aabb.max.x > sphere.c.x &&
		aabb.min.y < sphere.c.y && aabb.max.y > sphere.c.y &&
		aabb.min.z < sphere.c.z && aabb.max.z > sphere.c.z) {
		return true;
	}
	physics::Point closestPoint = {};
	physics::ClosestPtPointAABB(sphere.c, aabb, closestPoint);
	return physics::TestSpherePoint(sphere, closestPoint);
}

bool physics::TestAABBAABB(physics::AABB a, physics::AABB b) {
	// Exit with no intersection if separated along an axis
	if (a.max[0] < b.min[0] || a.min[0] > b.max[0]) { return false; }
	if (a.max[1] < b.min[1] || a.min[1] > b.max[1]) { return false; }
	if (a.max[2] < b.min[2] || a.min[2] > b.max[2]) { return false; }
	// Overlapping on all axes means AABBs are intersecting
	return true;
}

glm::vec3 physics::MoveSphereFromAABB(physics::Sphere sphere, physics::AABB aabb) {
	if (aabb.min.x <= sphere.c.x && aabb.max.x >= sphere.c.x &&
		aabb.min.y <= sphere.c.y && aabb.max.y >= sphere.c.y &&
		aabb.min.z <= sphere.c.z && aabb.max.z >= sphere.c.z) {
		physics::Point closestPoint = {};
		physics::ClosestPtPointAABB(sphere.c, aabb, closestPoint);
		physics::Vector sphereToPoint = closestPoint - sphere.c;
		if (glm::length(sphereToPoint) < FLT_EPSILON) {
			return glm::vec3{0, 0.01f, 0};
		}
		return sphereToPoint + (glm::normalize(sphereToPoint) * (sphere.r + (FLT_EPSILON * 3.0f)));
	} else {
		physics::Point closestPoint = {};
		physics::ClosestPtPointAABB(sphere.c, aabb, closestPoint);
		physics::Vector pointToSphere = sphere.c - closestPoint;
		if (glm::length(pointToSphere) < FLT_EPSILON) {
			return glm::vec3{0, 0.01f, 0};
		}
		return (glm::normalize(pointToSphere) * (sphere.r + (FLT_EPSILON * 3.0f))) - pointToSphere;
	}
}

glm::vec3 physics::MoveCapsuleFromAABB(physics::Capsule capsule, physics::AABB aabb) {
	glm::vec3 lastMove(-1000.0f);
	glm::vec3 totalMove(0);
	physics::Sphere sphere{capsule.a, capsule.r};
	bool hadCollision = true;
	for (int i = 0; i < 3 && hadCollision; i++) {
		hadCollision = false;
		// Check the "bottom" end cap
		sphere.c = capsule.b;
		if (physics::TestSphereAABB(sphere, aabb)) {
			hadCollision = true;
			glm::vec3 moveAmount = MoveSphereFromAABB(sphere, aabb);
			totalMove += moveAmount;
			capsule.a += moveAmount;
			capsule.b += moveAmount;
		}
		// Check the "top" end cap
		sphere.c = capsule.a;
		if (physics::TestSphereAABB(sphere, aabb)) {
			hadCollision = true;
			glm::vec3 moveAmount = MoveSphereFromAABB(sphere, aabb);
			totalMove += moveAmount;
			capsule.a += moveAmount;
			capsule.b += moveAmount;
		}
		// Check the middle
		sphere.c = capsule.b + ((capsule.a - capsule.b) / 2.0f);
		if (physics::TestSphereAABB(sphere, aabb)) {
			hadCollision = true;
			glm::vec3 moveAmount = MoveSphereFromAABB(sphere, aabb);
			totalMove += moveAmount;
			capsule.a += moveAmount;
			capsule.b += moveAmount;
		}
		if (glm::length(totalMove - lastMove) < FLT_EPSILON) {
			break;
		}
		lastMove = totalMove;
	}
	return totalMove;
}
