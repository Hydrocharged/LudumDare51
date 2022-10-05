// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <gui/gui.h>
#include <gui/fontmanager.h>
#include <cmath>
#include <array>

#if defined(PLATFORM_WEB)
const char* fontName = "assets/fonts/SourceCodePro-Regular.ttf";
#else
const char* fontName = "../assets/fonts/SourceCodePro-Regular.ttf";
#endif // PLATFORM_WEB

const int glyphCount = 128;
std::array<int, 21> fontSizes = {
	3,
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
	for (int i = 0; i < fontSizes.size(); i++) {
		fonts[i] = LoadFontEx(fontName, fontSizes[i], nullptr, glyphCount);
	}
}

void gui::fontmanager::Unload() {
	for (auto font: fonts) {
		UnloadFont(font);
	}
}

Font gui::fontmanager::GetBySize(float size) {
	return GetBySize((int)size);
}

Font gui::fontmanager::GetBySize(int size) {
	if (size <= fontSizes[0]) {
		return fonts[0];
	} else if (size >= fontSizes[fontSizes.size() - 1]) {
		return fonts[fonts.size() - 1];
	}

	int mid = 0;
	int low = 0;
	int high = fontSizes.size() - 1;
	while (high - low > 1) {
		mid = floor((low + high) / 2);
		if (fontSizes[mid] < size) {
			low = mid;
		} else {
			high = mid;
		}
	}
	if (size == fontSizes[low]) {
		return fonts[low];
	}
	return fonts[high];
}

Font gui::fontmanager::GetByWidth(float width, float maxHeight, float textLength, float& targetSize, float& widthAtSize) {
	if (maxHeight <= (float)fontSizes[0]) {
		return fonts[0];
	}

	int mid;
	int low = 0;
	int high = fontSizes.size() - 1;
	while (high - low > 1) {
		mid = floor((low + high) / 2.0);
		Font font = fonts[mid];
		float midLength = font.recs->width * (float)textLength;
		if (midLength < (float)width && font.recs->height <= maxHeight) {
			low = mid;
		} else {
			high = mid;
		}
	}
	Font font = fonts[high];

	float currentWidth = fonts[high].recs->width * (float)textLength;
	float currentToTargetWidth = width / currentWidth;
	float currentSize = font.recs->height * currentToTargetWidth;
	if (font.recs->height * currentToTargetWidth <= maxHeight) {
		targetSize = currentSize;
		widthAtSize = currentWidth * currentToTargetWidth;
	} else {
		targetSize = maxHeight;
		widthAtSize = currentWidth * (maxHeight / font.recs->height);
	}
	return font;
}
