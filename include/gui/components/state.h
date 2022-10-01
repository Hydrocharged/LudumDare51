// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef GUI_COMPONENTS_STATE_H
#define GUI_COMPONENTS_STATE_H
#include <map>

void Register(std::size_t id);
void Unregister(std::size_t id);
bool GetHoverState(std::size_t id);
void SetHoverState(std::size_t id, bool isHovering);
bool GetClickState(std::size_t id);
void SetClickState(std::size_t id, bool isDown);

#endif //GUI_COMPONENTS_STATE_H
