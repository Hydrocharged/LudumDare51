// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_FONTMANAGER_H
#define GUI_FONTMANAGER_H
#include <raylib.h>

namespace gui {
	namespace fontmanager {
		Font GetSize(float size);
		Font GetSize(int size);
	}
}

#endif //GUI_FONTMANAGER_H
