// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/dynamiclabel.h>
#include <gui/fontmanager.h>

void gui::DynamicLabel::Draw(float posX, float posY, float containerWidth, float containerHeight) {
	float width = Width(containerWidth, containerHeight);
	float height = Height(containerWidth, containerHeight);
	auto color = defaultColor;
	if (hoverColor.a > 0 && isHovering) {
		color = hoverColor;
	}

	std::string dynamicText = func();
	std::string comparisonText;
	if (text.empty()) {
		comparisonText = dynamicText;
	} else {
		comparisonText = text;
	}
	float outFontSize = 0.0f;
	float outTextWidth = 0.0f;
	Font font = gui::fontmanager::GetByWidth(width, height, (float)comparisonText.length(), outFontSize, outTextWidth);
	DrawTextEx(font, func().c_str(), Vector2{posX + (width / 2) - (outTextWidth / 2), posY + (height / 2) - (outFontSize / 2)},
		outFontSize, 0.0f, *reinterpret_cast<::Color*>(&color));
}
