#include "ConsoleSprite.h"
#include "ConsoleRenderer.h"
#include <string.h>

ConsoleSprite::ConsoleSprite(void) {
	buffer = NULL;
}

ConsoleSprite::~ConsoleSprite(void) {
	this->free();
}

void ConsoleSprite::free() {
	if(buffer)
		delete[] buffer;
}

void ConsoleSprite::init(int wid, int hei) {
	this->free();
	
	width = max(1, wid);
	height = max(1, hei);

	buffer = new CHAR_INFO[width*height];
}

int wstrlen(WCHAR *str) {
	int l = 0;
	while(str[l] != 0) l++;
	return l;
}

void ConsoleSprite::init(int wid, int frm, WCHAR str[], int att[]) {
	this->free();

	frames = max(1, frm);
	width = max(1, wid);
	int l = wstrlen(str) / frames;
	height = l / width;
	

	buffer = new CHAR_INFO[width*height*frames];

	int i;
	for(i=0; i<l*frames; i++) {
		buffer[i].Char.UnicodeChar = str[i];
		if(att)
			buffer[i].Attributes = att[i];
		else {
			buffer[i].Attributes = WHITE + BLACK * 16;
		}
	}
}

int ConsoleSprite::getNumFrames() {
	return frames;
}

void ConsoleSprite::recolor(WORD color) {
	int i;
	for(i=0; i<width*height*frames; i++) {
		if(buffer[i].Char.AsciiChar != ' ')
			buffer[i].Attributes = color;
	}
}

void ConsoleSprite::setColor(int i, WORD color) {
	buffer[i].Attributes = color;
}

int ConsoleSprite::getWidth() {
	return width;
}

int ConsoleSprite::getHeight() {
	return height;
}
