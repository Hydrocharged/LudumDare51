// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_FONTMANAGER_H
#define GUI_FONTMANAGER_H
#include <raylib.h>

namespace gui::fontmanager {
	Font GetBySize(float size);
	Font GetBySize(int size);
	Font GetByWidth(float width, float maxHeight, float textLength, float& fontSize, float& widthAtSize);
}

#endif //GUI_FONTMANAGER_H
