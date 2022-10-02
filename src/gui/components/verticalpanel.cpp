// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/verticalpanel.h>
#include <raylib.h>

float gui::VerticalPanel::Width(float containerWidth, float containerHeight) {
	return containerWidth * xScale;
}

float gui::VerticalPanel::Height(float containerWidth, float containerHeight) {
	return containerHeight * yScale;
}

gui::VerticalPanel* gui::VerticalPanel::SetXScale(float scale) {
	this->xScale = scale;
	return this;
}

gui::VerticalPanel* gui::VerticalPanel::SetYScale(float scale) {
	this->yScale = scale;
	return this;
}

gui::VerticalPanel* gui::VerticalPanel::SetColor(gui::Color color) {
	this->defaultColor = color;
	return this;
}

gui::VerticalPanel* gui::VerticalPanel::SetHoverColor(gui::Color color) {
	this->hoverColor = color;
	return this;
}

std::vector<gui::DrawRect> gui::VerticalPanel::ChildPositions(float posX, float posY, float containerWidth, float containerHeight) {
	std::vector<gui::DrawRect> positions(children.size());
	float width = Width(containerWidth, containerHeight);
	float height = Height(containerWidth, containerHeight);

	float totalChildHeight = 0;
	for (auto& child: children) {
		totalChildHeight += child->Height(width, height);
	}
	float childSpacing = (height - totalChildHeight) / ((float)(children.size()) + 1.0f);
	float nextChildY = posY;
	for (int i = 0; i < children.size(); i++) {
		nextChildY += childSpacing;
		positions[i] = DrawRect{posX + ((width - children[i]->Width(width, height)) / 2), nextChildY, width, height};
		nextChildY += children[i]->Height(width, height);
	}
	return positions;
}

void gui::VerticalPanel::Draw(float posX, float posY, float containerWidth, float containerHeight) {
	float width = Width(containerWidth, containerHeight);
	float height = Height(containerWidth, containerHeight);
	if (hoverColor.a > 0 && isHovering) {
		DrawRectangle((int)posX, (int)posY, (int)width, (int)height, *reinterpret_cast<::Color*>(&hoverColor));
	} else if (defaultColor.a > 0) {
		DrawRectangle((int)posX, (int)posY, (int)width, (int)height, *reinterpret_cast<::Color*>(&defaultColor));
	}

	auto childPositions = ChildPositions(posX, posY, containerWidth, containerHeight);
	for (int i = 0; i < childPositions.size(); i++) {
		auto rect = childPositions[i];
		children[i]->Draw(rect.PosX, rect.PosY, rect.ContainerWidth, rect.ContainerHeight);
	}
}
