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
	int fontSize = (int)height;
	int fontLength = MeasureText(text.c_str(), fontSize);
	float textLengthAt10 = (float)MeasureText(text.c_str(), 10);
	if (fontLength > width) {
		fontLength = width;
		fontSize = (int)(((float)fontLength / textLengthAt10) * 10.0f);
	}
	auto color = defaultColor;
	if (hoverColor.a > 0 && IsHovering) {
		color = hoverColor;
	}
	DrawTextEx(gui::fontmanager::GetSize(fontSize), text.c_str(), Vector2{posX + (width / 2) - (fontLength / 2), posY + (height / 2) - (fontSize / 2)},
		fontSize, 0.0f, *reinterpret_cast<::Color*>(&color));
}
