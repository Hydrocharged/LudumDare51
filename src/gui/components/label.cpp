// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/label.h>
#include <gui/fontmanager.h>

float gui::Label::Width(float containerWidth, float containerHeight) {
	return containerWidth * xScale;
}

float gui::Label::Height(float containerWidth, float containerHeight) {
	return containerHeight * yScale;
}

gui::Label* gui::Label::SetXScale(float scale) {
	this->xScale = scale;
	return this;
}

gui::Label* gui::Label::SetYScale(float scale) {
	this->yScale = scale;
	return this;
}

gui::Label* gui::Label::SetColor(gui::Color color) {
	this->defaultColor = color;
	return this;
}

gui::Label* gui::Label::SetHoverColor(gui::Color color) {
	this->hoverColor = color;
	return this;
}

std::vector<gui::DrawRect> gui::Label::ChildPositions(float posX, float posY, float containerWidth, float containerHeight) {
	return {};
}

void gui::Label::Draw(float posX, float posY, float containerWidth, float containerHeight) {
	float width = Width(containerWidth, containerHeight);
	float height = Height(containerWidth, containerHeight);
	auto color = defaultColor;
	if (hoverColor.a > 0 && isHovering) {
		color = hoverColor;
	}
	float outFontSize = 0.0f;
	float outTextWidth = 0.0f;
	Font font = gui::fontmanager::GetByWidth(width, height, (float)text.length(), outFontSize, outTextWidth);
	DrawTextEx(font, text.c_str(), Vector2{posX + (width / 2) - (outTextWidth / 2), posY + (height / 2) - (outFontSize / 2)},
		outFontSize, 0.0f, *reinterpret_cast<::Color*>(&color));
}
