// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/button.h>

bool gui::Button::HandleMouseUp(float xPercentage, float yPercentage) {
	if (callback != nullptr) {
		return callback();
	}
	return false;
}
