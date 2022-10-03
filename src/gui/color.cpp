// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/color.h>
#include <render/color.h>
#include <raylib.h>

gui::Color::Color(::Color color) {
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

gui::Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

gui::Color::operator ::Color() {
	return ::Color{r, g, b, a};
}

gui::Color::operator render::Color() {
	return render::Color{r, g, b, a};
}
