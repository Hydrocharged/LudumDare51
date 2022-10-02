// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COMPONENTS_PROGRESSBAR_H
#define GUI_COMPONENTS_PROGRESSBAR_H
#include <gui/components/horizontalpanel.h>

namespace gui {
	class ProgressBar : public HorizontalPanel {
	public:
		ProgressBar(float (* func)(), std::initializer_list<Component*> children) : HorizontalPanel(children), func(func) {}
		~ProgressBar() override = default;
		void Draw(float posX, float posY, float containerWidth, float containerHeight) override;

	protected:
		float (* func)();
	};

	inline ProgressBar* NewProgressBar(float (* func)(), std::initializer_list<Component*> children) { return new ProgressBar(func, children); }
}

#endif //GUI_COMPONENTS_PROGRESSBAR_H
