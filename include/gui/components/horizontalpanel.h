// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COMPONENTS_HORIZONTALPANEL_H
#define GUI_COMPONENTS_HORIZONTALPANEL_H
#include <gui/components/component.h>

namespace gui {
	class HorizontalPanel : public Component {
	public:
		HorizontalPanel(std::initializer_list<Component*> children) : Component(children) {}
		~HorizontalPanel() = default;
		std::vector<DrawRect> ChildPositions(float posX, float posY, float containerWidth, float containerHeight) override;
		void Draw(float posX, float posY, float containerWidth, float containerHeight) override;
		float Width(float containerWidth, float containerHeight) override;
		float Height(float containerWidth, float containerHeight) override;
		HorizontalPanel* SetXScale(float scale);
		HorizontalPanel* SetYScale(float scale);
		HorizontalPanel* SetColor(Color color);
		HorizontalPanel* SetHoverColor(Color color);
		HorizontalPanel* SetAlignment(Alignment align);

	protected:
		float xScale = 1.0f;
		float yScale = 1.0f;
		Color defaultColor = {0, 0, 0, 0};
		Color hoverColor = {0, 0, 0, 0};
		Alignment alignment = Alignment::Spread;
	};

	inline HorizontalPanel* NewHorizontalPanel(std::initializer_list<Component*> children) { return new HorizontalPanel(children); }
}

#endif //GUI_COMPONENTS_HORIZONTALPANEL_H
