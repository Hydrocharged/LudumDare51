// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COLOR_H
#define GUI_COLOR_H

struct Color;

namespace render {
	struct Color;
}

namespace gui {
	struct Color {
		Color() = default;
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		Color(::Color);
		unsigned char r; // Red
		unsigned char g; // Green
		unsigned char b; // Blue
		unsigned char a; // Alpha

		operator ::Color();
		operator render::Color();
	};
}

#endif //GUI_COLOR_H
