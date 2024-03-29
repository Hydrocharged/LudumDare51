// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COMPONENTS_COMPONENT_H
#define GUI_COMPONENTS_COMPONENT_H
#include <mouse.h>
#include <memory>
#include <functional>
#include <vector>
#include <gui/color.h>

namespace gui {
	enum class Alignment {
		Spread,    // Equal spacing between children and boundaries
		Justified, // Equal spacing between children, touches boundaries
		Center,    // All children grouped in the middle
		Start,     // All children grouped at the beginning
		End        // All children grouped at the end
	};

	struct DrawRect {
		float PosX;
		float PosY;
		float ContainerWidth;
		float ContainerHeight;
	};

	class Component {
	public:
		Component(std::initializer_list<Component*> children);
		virtual ~Component();

		virtual std::vector<DrawRect> ChildPositions(float posX, float posY, float containerWidth, float containerHeight) = 0;
		virtual void Draw(float posX, float posY, float containerWidth, float containerHeight) = 0;
		virtual float Width(float containerWidth, float containerHeight) = 0;
		virtual float Height(float containerWidth, float containerHeight) = 0;
		virtual void HoverEnter() {}
		virtual void HoverExit() {}
		virtual void HandleUpdate() {}
		virtual bool HandleMouseDown(float xPercentage, float yPercentage) { return false; }
		virtual bool HandleMouseUp(float xPercentage, float yPercentage) { return false; }
		virtual bool HandleMouseDrag(float prevXPerc, float prevYPerc, float xPerc, float yPerc) { return false; }
		virtual void MouseDownCancelled() {}

		void Update(mouse::Info& mouse, DrawRect& rect);

		void AddChild(Component* child) { this->children.push_back(std::unique_ptr<gui::Component>(child)); }
		Component* operator[](int i) { return children.at(i).get(); }
		Component* Child(int i) { return children.at(i).get(); }
		size_t ChildCount() { return children.size(); }
		size_t ID() { return reinterpret_cast<size_t>(this); }

	protected:
		bool isHovering = false;
		bool isMouseDown = false;
		std::vector<std::unique_ptr<Component>> children;

	private:
		struct Events {
			bool MouseDownWasHandled = false;
			bool MouseDragWasHandled = false;
			bool MouseUpWasHandled = false;
		};
		void update(mouse::Info& mouse, DrawRect& rect, Events& events);
	};
}

#endif //GUI_COMPONENTS_COMPONENT_H
