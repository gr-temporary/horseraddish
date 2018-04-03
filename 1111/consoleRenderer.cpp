#include "ConsoleRenderer.h"
#include "ConsoleSprite.h"

ConsoleRenderer::ConsoleRenderer(void) {

}

ConsoleRenderer::~ConsoleRenderer(void) {
	CloseHandle(buf[0]);
	CloseHandle(buf[1]);
}

void ConsoleRenderer::setTitle(LPCWSTR title) {
	SetConsoleTitleW(title);
}

void ConsoleRenderer::init() {
	SetConsoleOutputCP(65001);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;
	cfi.dwFontSize.Y = 12;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Terminal");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	buf[0] = CreateConsoleScreenBuffer(GENERIC_READ+GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	buf[1] = CreateConsoleScreenBuffer(GENERIC_READ+GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	SetConsoleMode(buf[0], ENABLE_PROCESSED_INPUT + ENABLE_MOUSE_INPUT);
	SetConsoleMode(buf[1], ENABLE_PROCESSED_INPUT + ENABLE_MOUSE_INPUT);

	width = SetConsoleCursorInfo(buf[0], &cci);
	width = SetConsoleCursorInfo(buf[1], &cci);

	inputHnd = GetStdHandle(STD_INPUT_HANDLE);

	width = BUFFER_WIDTH;
	height = BUFFER_HEIGHT;

	setActiveAttr(WHITE + BLACK * 16);

	activeIndex = 0;
	clear();
	swap();
}

void ConsoleRenderer::swap() {
	COORD size, from;
	size.X = width; 
	size.Y = height;
	from.X = 0;
	from.Y = 0;
	SMALL_RECT writeRegion = {0, 0, BUFFER_WIDTH-1, BUFFER_HEIGHT-1};
	
	activeHnd = buf[activeIndex];
	if(WriteConsoleOutput(activeHnd, (CHAR_INFO*)buffer, size, from, &writeRegion))
		SetConsoleActiveScreenBuffer(activeHnd);
	activeIndex = ( ++activeIndex ) % 2;
	activeHnd = buf[activeIndex];
}

void ConsoleRenderer::clear(WCHAR c, WORD col) {
	int i, j;
	for(i=0; i<width; i++)
		for(j=0; j<height; j++) {
			buffer[j][i].Char.UnicodeChar = c;
			buffer[j][i].Attributes = col;
		}
}

DWORD ConsoleRenderer::getKeyPressed() {
	INPUT_RECORD ir;
	DWORD d;
	GetNumberOfConsoleInputEvents(inputHnd, &d);
	if(d > 0) {
		ReadConsoleInput(inputHnd, &ir, 1, &d);
		if(ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
			return ir.Event.KeyEvent.wVirtualKeyCode;
		}
	}
	return 0;
}

void ConsoleRenderer::puts(int x, int y, WCHAR *s) {
	/*_COORD c;
	c.X = x;
	c.Y = y;
	DWORD d;
	char ts[256];
	int l = strlen(s);
	CharToOem(s, ts);
	WriteConsoleOutputCharacter(activeHnd, ts, l, c, &d);
	FillConsoleOutputAttribute(activeHnd, activeAttr, l, c, &d);*/
}

void ConsoleRenderer::setBackColor(WORD c) {
	activeAttr = (activeAttr & 0xF) + (c * 16) ;
}

void ConsoleRenderer::setBackColor(BOOL r, BOOL g, BOOL b, BOOL l) {
	activeAttr = (activeAttr & 0xF) + (l * 16 + b * 32 + g * 64 + r * 128) ;
}

void ConsoleRenderer::setTextColor(WORD c) {
	activeAttr = (activeAttr & 0xF0) + c ;
}

void ConsoleRenderer::setTextColor(BOOL r, BOOL g, BOOL b, BOOL l) {
	activeAttr = (activeAttr & 0xF0) + (l + b * 2 + g * 4 + r * 8);
}

void ConsoleRenderer::setActiveAttr(WORD attr) {
	activeAttr = attr;
}

inline bool ConsoleRenderer::inside(int x, int y) {
	return (x >= 0) && (x < width) && (y >= 0) && (y < height);
}

void ConsoleRenderer::setAttr(int x, int y, WORD c) {
	if(inside(x,y))
		buffer[y][x].Attributes = c;
}

void ConsoleRenderer::setChar(int x, int y, WCHAR c) {
	if(inside(x,y))
		buffer[y][x].Char.UnicodeChar = c;
}

void ConsoleRenderer::setCell(int x, int y, WCHAR c, WORD attr) {
	if(inside(x,y)) {
		buffer[y][x].Attributes = attr;
		buffer[y][x].Char.UnicodeChar = c;
	}
}

WCHAR ConsoleRenderer::getChar(int x, int y) {
	if(inside(x,y))
		return buffer[y][x].Char.UnicodeChar;
	return 0;
}

WORD ConsoleRenderer::getAttr(int x, int y) {
	if(inside(x,y))
		return buffer[y][x].Attributes;
	return 0;
}

void ConsoleRenderer::drawSprite(ConsoleSprite *spr, int x, int y, int frame, bool hurt) {
	if(!spr)
		return;

	int i, ix, iy, w, h, l;
	w = spr->width;
	h = spr->height;
	l = w * h;
	int f = spr->frames;

	for(i=0; i<l; i++) {
		ix = x + i % w;
		iy = y + i / w;
		if(spr->buffer[l*frame + i].Char.UnicodeChar != ' ' && inside(ix, iy)) {
			buffer[iy][ix] = spr->buffer[l*frame + i];
			if(hurt)
				buffer[iy][ix].Attributes = !buffer[iy][ix].Attributes;
			/*buffer[iy][ix].Char = spr->buffer[l*frame + i].Char;
			if(spr->buffer[l*frame + i].Attributes / 16 != 0)
				buffer[iy][ix].Attributes = spr->buffer[l*frame + i].Attributes;
			else 
				buffer[iy][ix].Attributes = buffer[iy][ix].Attributes & 0xF0 + spr->buffer[l*frame + i].Attributes;*/
		}
	}

}