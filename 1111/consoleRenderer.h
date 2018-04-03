#pragma once
#include <windows.h>
#include <string.h>
#include <locale.h>
#include "ConsoleSprite.h"
#include "consts.h"

class ConsoleRenderer
{
	HANDLE buf[2];
	HANDLE activeHnd;
	HANDLE inputHnd;
	int activeIndex;

	CHAR_INFO buffer[BUFFER_HEIGHT][BUFFER_WIDTH];

	int width;
	int height;

	WORD activeAttr;

public:
	ConsoleRenderer(void);
	~ConsoleRenderer(void);

	void init();
	void swap();

	// key input
	DWORD getKeyPressed();

	void setTitle(LPCWSTR title);

	// drawing routines
	void clear(WCHAR c = ' ', WORD col = 0);

	void puts(int x, int y, WCHAR *s);
	void setChar(int x, int y, WCHAR c);
	void setCell(int x, int y, WCHAR c, WORD attr);
	WCHAR getChar(int x, int y);
	void setAttr(int x, int y, WORD c);
	WORD getAttr(int x, int y);

	void setActiveAttr(WORD attr);
	void setTextColor(WORD c);
	void setTextColor(BOOL r, BOOL g, BOOL b, BOOL l);
	void setBackColor(WORD c);
	void setBackColor(BOOL r, BOOL g, BOOL b, BOOL l);

	void drawSprite(ConsoleSprite *spr, int x, int y, int frame = 0, bool hurt = false);

	bool inside(int x, int y);
};

//inline WORD conColor(WORD br, WORD bg, WORD bb, WORD bl, WORD fr, WORD fg, WORD fb, WORD fl) {
//	return br << 7 + bg << 6 + bb << 5 + bl << 4 + fr << 3 + fg << 2 + fb << 1 + fl;
//}
