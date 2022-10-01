// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifdef _WIN32
#include <hideconsole.h>
#include <windows.h>

void HideConsole() {
	FreeConsole();
}

#endif //_WIN32
