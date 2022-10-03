// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef RENDER_PRIMITIVES_H
#define RENDER_PRIMITIVES_H
#include <render/color.h>
#include <physics/body.h>

namespace render {
	void Body(physics::Body* body, render::Color color);
	void BodyWireframe(physics::Body* body, render::Color color);
	void AABB(physics::AABBBody* body, render::Color color);
	void AABB(physics::AABB aabb, render::Color color);
	void AABBWireframe(physics::AABBBody* body, render::Color color);
	void AABBWireframe(physics::AABB aabb, render::Color color);
	void Sphere(physics::SphereBody* body, render::Color color);
	void Sphere(physics::Sphere sphere, render::Color color);
	void SphereWireframe(physics::SphereBody* body, render::Color color, int rings, int slices);
	void SphereWireframe(physics::Sphere sphere, render::Color color, int rings, int slices);
	void Capsule(physics::CapsuleBody* body, render::Color color);
	void Capsule(physics::Capsule capsule, render::Color color);
	void CapsuleWireframe(physics::CapsuleBody* body, render::Color color, int rings, int slices);
	void CapsuleWireframe(physics::Capsule capsule, render::Color color, int rings, int slices);
}

#endif //RENDER_PRIMITIVES_H
