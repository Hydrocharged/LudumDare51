// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/gui.h>
#include <gui/fontmanager.h>
#include <cmath>

const char* fontName = "../assets/fonts/SourceCodePro-Regular.ttf";
const int glyphCount = 128;
std::array<int, 21> fontSizes = {
	2,
	4,
	6,
	8,
	10,
	12,
	14,
	16,
	20,
	24,
	28,
	32,
	36,
	40,
	48,
	56,
	64,
	72,
	84,
	128,
	256,
};
std::array<Font, fontSizes.size()> fonts;

void gui::fontmanager::Load() {
	for(int i = 0; i < fontSizes.size(); i++) {
		fonts[i] = LoadFontEx(fontName, fontSizes[i], nullptr, glyphCount);
	}
}

void gui::fontmanager::Unload() {
	for (auto font: fonts) {
		UnloadFont(font);
	}
}

Font gui::fontmanager::GetSize(float size) {
	return GetSize((int)size);
}

Font gui::fontmanager::GetSize(int size) {
	if(size <= fontSizes[0]) {
		return fonts[0];
	} else if(size >= fontSizes[fontSizes.size()-1]) {
		return fonts[fonts.size()-1];
	}

	int mid = 0;
	int low = 0;
	int high = fontSizes.size() - 1;
	while(high - low > 1) {
		mid = floor((low + high) / 2);
		if(fontSizes[mid] < size) {
			low = mid;
		} else {
			high = mid;
		}
	}
	if(size == fontSizes[low]) {
		return fonts[low];
	}
	return fonts[high];
}
