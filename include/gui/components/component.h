// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COMPONENTS_COMPONENT_H
#define GUI_COMPONENTS_COMPONENT_H
#include <memory>
#include <functional>

namespace gui {
	class Component {
	public:
		Component(std::initializer_list<Component*> children);
		~Component();

		virtual void Draw(float posX, float posY, float containerWidth, float containerHeight) = 0;
		virtual float Width(float containerWidth, float containerHeight) = 0;
		virtual float Height(float containerWidth, float containerHeight) = 0;
		virtual void HoverEnter() {}
		virtual void HoverExit() {}
		virtual bool HandleMouseDown(float xPercentage, float yPercentage) { return false; }
		virtual bool HandleMouseDrag(float prevXPerc, float prevYPerc, float xPerc, float yPerc) { return false; }
		virtual bool HandleMouseUp(float xPercentage, float yPercentage) { return false; }
		virtual bool HandleMouseClick(float xPercentage, float yPercentage) { return false; }

		void AddChild(Component* child) { this->children.push_back(std::unique_ptr<gui::Component>(child)); }
		Component* operator[](int i) { return children.at(i).get(); }
		Component* Child(int i) { return children.at(i).get(); }
		size_t ChildCount() { return children.size(); }
		size_t ID() { return reinterpret_cast<size_t>(this); }

	protected:
		bool IsHovering();
		bool IsMouseDown();
		std::vector<std::unique_ptr<Component>> children;
	};

	struct Color {
		unsigned char r; // Red
		unsigned char g; // Green
		unsigned char b; // Blue
		unsigned char a; // Alpha
	};
}

#endif //GUI_COMPONENTS_COMPONENT_H
