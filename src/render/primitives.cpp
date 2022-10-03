// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <render/primitives.h>
#include <raylib.h>

void render::Body(physics::Body* body, render::Color color) {
	switch (body->ColliderType()) {
		case physics::ColliderType::Sphere: {
			render::Sphere(static_cast<physics::SphereBody*>(body), color);
			break;
		}
		case physics::ColliderType::Capsule: {
			render::Capsule(static_cast<physics::CapsuleBody*>(body), color);
			break;
		}
		case physics::ColliderType::AABB: {
			render::AABB(static_cast<physics::AABBBody*>(body), color);
			break;
		}
	}
}

void render::BodyWireframe(physics::Body* body, render::Color color) {
	switch (body->ColliderType()) {
		case physics::ColliderType::Sphere: {
			render::SphereWireframe(static_cast<physics::SphereBody*>(body), color, 20, 20);
			break;
		}
		case physics::ColliderType::Capsule: {
			render::CapsuleWireframe(static_cast<physics::CapsuleBody*>(body), color, 20, 20);
			break;
		}
		case physics::ColliderType::AABB: {
			render::AABBWireframe(static_cast<physics::AABBBody*>(body), color);
			break;
		}
	}
}

void render::AABB(physics::AABBBody* body, render::Color color) {
	render::AABB(body->GetAABB(), color);
}

void render::AABB(physics::AABB aabb, render::Color color) {
	float width = aabb.max.x - aabb.min.x;
	float height = aabb.max.y - aabb.min.y;
	float length = aabb.max.z - aabb.min.z;
	::DrawCube(Vector3{aabb.min.x + (width / 2.0f), aabb.min.y + (height / 2.0f), aabb.min.z + (length / 2.0f)},
		width, height, length, color);
}

void render::AABBWireframe(physics::AABBBody* body, render::Color color) {
	render::AABBWireframe(body->GetAABB(), color);
}

void render::AABBWireframe(physics::AABB aabb, render::Color color) {
	float width = aabb.max.x - aabb.min.x;
	float height = aabb.max.y - aabb.min.y;
	float length = aabb.max.z - aabb.min.z;
	::DrawCubeWires(Vector3{aabb.min.x + (width / 2.0f), aabb.min.y + (height / 2.0f), aabb.min.z + (length / 2.0f)},
		width, height, length, color);
}

void render::Sphere(physics::SphereBody* body, render::Color color) {
	render::Sphere(body->GetSphere(), color);
}

void render::Sphere(physics::Sphere sphere, render::Color color) {
	::DrawSphere(Vector3{sphere.c.x, sphere.c.y, sphere.c.z}, sphere.r, color);
}

void render::SphereWireframe(physics::SphereBody* body, render::Color color, int rings, int slices) {
	render::SphereWireframe(body->GetSphere(), color, rings, slices);
}

void render::SphereWireframe(physics::Sphere sphere, render::Color color, int rings, int slices) {
	::DrawSphereWires(Vector3{sphere.c.x, sphere.c.y, sphere.c.z}, sphere.r, rings, slices, color);
}

void render::Capsule(physics::CapsuleBody* body, render::Color color) {
	render::Capsule(body->GetCapsule(), color);
}

void render::Capsule(physics::Capsule capsule, render::Color color) {
	render::Sphere(physics::Sphere{capsule.a, capsule.r}, color);
	render::Sphere(physics::Sphere{capsule.b, capsule.r}, color);
	::DrawCylinderEx(Vector3{capsule.a.x, capsule.a.y, capsule.a.z},
		Vector3{capsule.b.x, capsule.b.y, capsule.b.z}, capsule.r, capsule.r, 20, color);
}

void render::CapsuleWireframe(physics::CapsuleBody* body, render::Color color, int rings, int slices) {
	render::CapsuleWireframe(body->GetCapsule(), color, rings, slices);
}

void render::CapsuleWireframe(physics::Capsule capsule, render::Color color, int rings, int slices) {
	render::SphereWireframe(physics::Sphere{capsule.a, capsule.r}, color, rings, slices);
	render::SphereWireframe(physics::Sphere{capsule.b, capsule.r}, color, rings, slices);
	::DrawCylinderWiresEx(Vector3{capsule.a.x, capsule.a.y, capsule.a.z},
		Vector3{capsule.b.x, capsule.b.y, capsule.b.z}, capsule.r, capsule.r, rings, color);
}
