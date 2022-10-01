// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COMPONENTS_VERTICALPANEL_H
#define GUI_COMPONENTS_VERTICALPANEL_H
#include <gui/components/component.h>

namespace gui {
	class VerticalPanel : public Component {
	public:
		VerticalPanel(std::initializer_list<Component*> children) : Component(children) {}
		~VerticalPanel() = default;
		std::vector<DrawRect> ChildPositions(float posX, float posY, float containerWidth, float containerHeight) override;
		void Draw(float posX, float posY, float containerWidth, float containerHeight) override;
		float Width(float containerWidth, float containerHeight) override;
		float Height(float containerWidth, float containerHeight) override;
		VerticalPanel* SetXScale(float scale);
		VerticalPanel* SetYScale(float scale);
		VerticalPanel* SetColor(Color color);
		VerticalPanel* SetHoverColor(Color color);

	private:
		float xScale = 1.0f;
		float yScale = 1.0f;
		Color defaultColor = {0, 0, 0, 0};
		Color hoverColor = {0, 0, 0, 0};
	};

	inline VerticalPanel* NewVerticalPanel(std::initializer_list<Component*> children) { return new VerticalPanel(children); }
}

#endif //GUI_COMPONENTS_VERTICALPANEL_H
