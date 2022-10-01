// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/state.h>

std::map<std::size_t, bool> hover = {};
std::map<std::size_t, bool> click = {};

void Register(std::size_t id) {
	hover.emplace(id, false);
	click.emplace(id, false);
}

void Unregister(std::size_t id) {
	hover.erase(id);
	click.erase(id);
}

bool GetHoverState(std::size_t id) {
	return hover[id];
}

void SetHoverState(std::size_t id, bool isHovering) {
	hover[id] = isHovering;
}

bool GetClickState(std::size_t id) {
	return click[id];
}

void SetClickState(std::size_t id, bool isDown) {
	click[id] = isDown;
}
