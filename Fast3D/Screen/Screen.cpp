#include "Screen.h"
using namespace Console3D;

Screen::Screen() : ActiveBuffer(0) {
	this->Buffers[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	this->Buffers[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	screen = nullptr;

	SetCursor(1, false);
	SetScreenNow();
}

void Screen::SetCursor(int size, bool visible) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(this->Buffers[0], &cursorInfo);
	cursorInfo.dwSize = size;
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(this->Buffers[0], &cursorInfo);
	SetConsoleCursorInfo(this->Buffers[1], &cursorInfo);
}

void Screen::SetScreenNow() {
	CONSOLE_SCREEN_BUFFER_INFO Info;
	GetConsoleScreenBufferInfo(this->Buffers[this->ActiveBuffer], &Info);

	int newW = Info.srWindow.Right - Info.srWindow.Left + 1;
	int newH = Info.srWindow.Bottom - Info.srWindow.Top + 1;

	if (newW != this->width || newH != this->height) {
		this->width = newW;
		this->height = newH;
		this->SetAspect();

		if (screen != nullptr) {
			delete[] screen;
			screen = nullptr;
		}
		screen = new wchar_t[this->width * this->height];

		COORD coord = { this->width, this->height };
		SetConsoleScreenBufferSize(this->Buffers[0], coord);
		SetConsoleScreenBufferSize(this->Buffers[1], coord);

		SMALL_RECT rect = { 0, 0, this->width - 1, this->height - 1 };
		SetConsoleWindowInfo(this->Buffers[0], TRUE, &rect);
		SetConsoleWindowInfo(this->Buffers[1], TRUE, &rect);
	}

	FreeScreen();
}

void Screen::SetAspect() {
	AspectRatio = (float)this->height / (float)this->width;
	PixelRatio = 24.f / 11.f;
}

void Screen::FreeScreen() {
	if (screen != nullptr) {
		wmemset(screen, L' ', width * height);
	}
}

void Screen::SwapBuffers() {
	SetConsoleActiveScreenBuffer(this->Buffers[1 - this->ActiveBuffer]);
	this->ActiveBuffer = 1 - this->ActiveBuffer;
	FreeScreen();
}

int Screen::width = 0;
int Screen::height = 0;
float Screen::AspectRatio = 1.0f;
float Screen::PixelRatio = 1.0f;
wchar_t* Screen::screen = nullptr;