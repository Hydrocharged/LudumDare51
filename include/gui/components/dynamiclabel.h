// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COMPONENTS_DYNAMICLABEL_H
#define GUI_COMPONENTS_DYNAMICLABEL_H
#include <gui/components/label.h>
#include <utility>

namespace gui {
	class DynamicLabel : public Label {
	public:
		explicit DynamicLabel(std::string strTemplate, std::string (* func)()) : Label(std::move(strTemplate)), func(func) {}
		~DynamicLabel() override = default;
		void Draw(float posX, float posY, float containerWidth, float containerHeight) override;

	protected:
		std::string (* func)();
	};

	inline DynamicLabel* NewDynamicLabel(std::string strTemplate, std::string (* func)()) { return new DynamicLabel(std::move(strTemplate), func); }
}

#endif //GUI_COMPONENTS_DYNAMICLABEL_H
