#pragma once
#include <iostream>
#include <Windows.h>

namespace Console3D {

struct Screen {
	static int width, height;
	static float AspectRatio, PixelRatio;
	static wchar_t* screen;

	HANDLE Buffers[2];
	int ActiveBuffer;

	Screen();

	void SetCursor(int size, bool visible);
	void SetScreenNow();
	void SetAspect();
	void FreeScreen();
	void SwapBuffers();
};

}