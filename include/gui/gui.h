// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_GUI_H
#define GUI_GUI_H
#include <gui/components/component.h>
#include <gui/components/horizontalpanel.h>
#include <gui/components/verticalpanel.h>
#include <gui/components/label.h>
#include <gui/components/button.h>
#include <gui/components/dynamiclabel.h>
#include <gui/components/progressbar.h>

namespace gui::fontmanager {
	void Load();
	void Unload();
}

#endif //GUI_GUI_H
