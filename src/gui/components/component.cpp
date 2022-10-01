// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/component.h>
#include <gui/components/state.h>

gui::Component::Component(std::initializer_list<Component*> children) {
	Register(ID());
	for (auto child: children) {
		this->children.push_back(std::unique_ptr<gui::Component>(child));
	}
}

gui::Component::~Component() {
	Unregister(ID());
}

bool gui::Component::IsHovering() {
	return GetHoverState(ID());
}

bool gui::Component::IsMouseDown() {
	return GetClickState(ID());
}
