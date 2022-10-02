// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/progressbar.h>
#include <raylib.h>

void gui::ProgressBar::Draw(float posX, float posY, float containerWidth, float containerHeight) {
	float width = Width(containerWidth, containerHeight);
	float height = Height(containerWidth, containerHeight);
	if (hoverColor.a > 0 && isHovering) {
		DrawRectangle((int)posX, (int)posY, (int)(width * func()), (int)height, *reinterpret_cast<::Color*>(&hoverColor));
	} else if (defaultColor.a > 0) {
		DrawRectangle((int)posX, (int)posY, (int)(width * func()), (int)height, *reinterpret_cast<::Color*>(&defaultColor));
	}

	auto childPositions = ChildPositions(posX, posY, containerWidth, containerHeight);
	for (int i = 0; i < childPositions.size(); i++) {
		auto rect = childPositions[i];
		children[i]->Draw(rect.PosX, rect.PosY, rect.ContainerWidth, rect.ContainerHeight);
	}
}
