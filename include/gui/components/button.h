// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COMPONENTS_BUTTON_H
#define GUI_COMPONENTS_BUTTON_H
#include <gui/components/horizontalpanel.h>

namespace gui {
	class Button : public HorizontalPanel {
	public:
		Button(bool (* callback)(), std::initializer_list<Component*> children) : HorizontalPanel(children), callback(callback) {}
		~Button() override = default;
		bool HandleMouseUp(float xPercentage, float yPercentage) override;

	protected:
		bool (* callback)();
	};

	inline Button* NewButton(bool (* callback)(), std::initializer_list<Component*> children) { return new Button(callback, children); }
}

#endif //GUI_COMPONENTS_BUTTON_H
