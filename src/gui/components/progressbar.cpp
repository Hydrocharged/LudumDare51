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
	float drawX = posX;
	float drawWidth = width * func();
	switch (alignment) {
		case Alignment::Spread:
		case Alignment::Justified:
		case Alignment::Center:
			drawX += (width - drawWidth) / 2.0f;
			break;
		case Alignment::Start:
			break;
		case Alignment::End:
			drawX += width - drawWidth;
			break;
	}
	if (hoverColor.a > 0 && isHovering) {
		DrawRectangle((int)drawX, (int)posY, (int)drawWidth, (int)height, *reinterpret_cast<::Color*>(&hoverColor));
	} else if (defaultColor.a > 0) {
		DrawRectangle((int)drawX, (int)posY, (int)drawWidth, (int)height, *reinterpret_cast<::Color*>(&defaultColor));
	}

	auto childPositions = ChildPositions(posX, posY, containerWidth, containerHeight);
	for (int i = 0; i < childPositions.size(); i++) {
		auto rect = childPositions[i];
		children[i]->Draw(rect.PosX, rect.PosY, rect.ContainerWidth, rect.ContainerHeight);
	}
}
