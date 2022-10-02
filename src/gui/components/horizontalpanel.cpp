// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/horizontalpanel.h>
#include <raylib.h>

float gui::HorizontalPanel::Width(float containerWidth, float containerHeight) {
	return containerWidth * xScale;
}

float gui::HorizontalPanel::Height(float containerWidth, float containerHeight) {
	return containerHeight * yScale;
}

gui::HorizontalPanel* gui::HorizontalPanel::SetXScale(float scale) {
	this->xScale = scale;
	return this;
}

gui::HorizontalPanel* gui::HorizontalPanel::SetYScale(float scale) {
	this->yScale = scale;
	return this;
}

gui::HorizontalPanel* gui::HorizontalPanel::SetColor(gui::Color color) {
	this->defaultColor = color;
	return this;
}

gui::HorizontalPanel* gui::HorizontalPanel::SetHoverColor(gui::Color color) {
	this->hoverColor = color;
	return this;
}

std::vector<gui::DrawRect> gui::HorizontalPanel::ChildPositions(float posX, float posY, float containerWidth, float containerHeight) {
	std::vector<gui::DrawRect> positions(children.size());
	float width = Width(containerWidth, containerHeight);
	float height = Height(containerWidth, containerHeight);

	float totalChildWidth = 0;
	for (auto& child: children) {
		totalChildWidth += child->Width(width, height);
	}
	float childSpacing = (width - totalChildWidth) / ((float)(children.size()) + 1.0f);
	float nextChildX = posX;
	for (int i = 0; i < children.size(); i++) {
		nextChildX += childSpacing;
		positions[i] = DrawRect{nextChildX, posY + ((height - children[i]->Height(width, height)) / 2), width, height};
		nextChildX += children[i]->Width(width, height);
	}
	return positions;
}

void gui::HorizontalPanel::Draw(float posX, float posY, float containerWidth, float containerHeight) {
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
