// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COMPONENTS_LABEL_H
#define GUI_COMPONENTS_LABEL_H
#include <gui/components/component.h>
#include <string>
#include <utility>

namespace gui {
	class Label : public Component {
	public:
		explicit Label(std::string text) : Component({}) { this->text = std::move(text); }
		~Label() = default;
		std::vector<DrawRect> ChildPositions(float posX, float posY, float containerWidth, float containerHeight) override;
		void Draw(float posX, float posY, float containerWidth, float containerHeight) override;
		float Width(float containerWidth, float containerHeight) override;
		float Height(float containerWidth, float containerHeight) override;
		Label* SetXScale(float scale);
		Label* SetYScale(float scale);
		Label* SetColor(Color color);
		Label* SetHoverColor(Color color);

	protected:
		std::string text;
		float xScale = 1.0f;
		float yScale = 1.0f;
		Color defaultColor = {0, 0, 0, 255};
		Color hoverColor = {0, 0, 0, 255};
	};

	inline Label* NewLabel(std::string text) { return new Label(std::move(text)); }
}

#endif //GUI_COMPONENTS_LABEL_H
