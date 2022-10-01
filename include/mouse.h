// Copyright © 2022 Daylon Wilkins

#ifndef MOUSE_H
#define MOUSE_H

namespace mouse {
	struct Info {
		float PrevX = 0.0f;
		float PrevY = 0.0f;
		float CurrentX = 0.0f;
		float CurrentY = 0.0f;
		bool IsMouseDown = false;
		bool IsMouseDrag = false;

		void Update();
	};
}

#endif //MOUSE_H
