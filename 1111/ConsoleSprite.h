#pragma once
#include <windows.h>
#include "consts.h"

class ConsoleSprite
{
	friend class ConsoleRenderer;
	CHAR_INFO *buffer;

	int width;
	int height;
	int frames;

public:
	ConsoleSprite(void);
	~ConsoleSprite(void);

	void init(int wid, int hei);
	void init(int wid, int frm, WCHAR str[], int att[]);
	void free();

	void recolor(WORD color);
	void setColor(int i, WORD color);

	int getNumFrames();
	int getWidth();
	int getHeight();
	/*void setText(char *str);
	void setAttr(CHAR *att);*/
};
