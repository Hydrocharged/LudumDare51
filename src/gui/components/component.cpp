// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/components/component.h>

gui::Component::Component(std::initializer_list<Component*> children) {
	for (auto child: children) {
		this->children.push_back(std::unique_ptr<gui::Component>(child));
	}
}

gui::Component::~Component() = default;

void gui::Component::Update(MouseInfo& mouse, DrawRect& rect) {
	Events events;
	update(mouse, rect, events);
}

inline float clamp(float val) {
	if (val < 0.0f) {
		return 0.0f;
	}
	if (val > 1.0f) {
		return 1.0f;
	}
	return val;
}

void gui::Component::update(MouseInfo& mouse, DrawRect& rect, Events& events) {
	float x = rect.PosX;
	float y = rect.PosY;
	float width = Width(rect.ContainerWidth, rect.ContainerHeight);
	float height = Height(rect.ContainerWidth, rect.ContainerHeight);
	float prevXPercentage = (mouse.PrevX - x) / rect.ContainerWidth;
	float prevYPercentage = (mouse.PrevY - y) / rect.ContainerHeight;
	float xPercentage = (mouse.CurrentX - x) / rect.ContainerWidth;
	float yPercentage = (mouse.CurrentY - y) / rect.ContainerHeight;
	bool withinBounds = x <= mouse.CurrentX && mouse.CurrentX <= (x + width) && y <= mouse.CurrentY && mouse.CurrentY <= (y + height);

	// Iterate Children First
	auto childPositions = ChildPositions(rect.PosX, rect.PosY, rect.ContainerWidth, rect.ContainerHeight);
	for (int i = 0; i < childPositions.size(); i++) {
		children[i]->update(mouse, childPositions[i], events);
	}

	// Handle Hover
	if (withinBounds && !IsHovering) {
		IsHovering = true;
		HoverEnter();
	} else if (!withinBounds && IsHovering) {
		IsHovering = false;
		HoverExit();
	}

	// Handle Clicks
	if (mouse.IsMouseDown) {
		if (withinBounds) {
			if (IsMouseDown) {
				if (!events.MouseDragWasHandled) {
					events.MouseDragWasHandled = HandleMouseDrag(prevXPercentage, prevYPercentage, xPercentage, yPercentage);
				}
			} else if (!mouse.IsMouseDrag) {
				if (!events.MouseDownWasHandled) {
					events.MouseDownWasHandled = HandleMouseDown(xPercentage, yPercentage);
					IsMouseDown = true;
				}
			}
		} else if (IsMouseDown && mouse.IsMouseDrag) {
			if (!events.MouseDragWasHandled) {
				events.MouseDragWasHandled = HandleMouseDrag(clamp(prevXPercentage), clamp(prevYPercentage), clamp(xPercentage), clamp(yPercentage));
			}
		}
	} else {
		if (IsMouseDown) {
			if (withinBounds) {
				if (!events.MouseUpWasHandled) {
					events.MouseUpWasHandled = HandleMouseUp(xPercentage, yPercentage);
				} else {
					MouseDownCancelled();
				}
			} else {
				MouseDownCancelled();
			}
			IsMouseDown = false;
		}
	}
}
