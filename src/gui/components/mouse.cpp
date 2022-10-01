// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/component.h>
#include <raylib.h>

void MouseInfo::Update() {
	PrevX = CurrentX;
	PrevY = CurrentY;
	CurrentX = (float)GetMouseX();
	CurrentY = (float)GetMouseY();
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		if (IsMouseDown) {
			IsMouseDrag = true;
		} else {
			IsMouseDown = true;
		}
	} else {
		IsMouseDown = false;
		IsMouseDrag = false;
	}
}
