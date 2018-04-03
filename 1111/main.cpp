#include <stdio.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <time.h>
#include "consoleRenderer.h"

char    vowels[]  = {'a','e','i','o','u','y'};      
char    consontants[]    = {'b','c','d','f','g','h','j','k','l','m','n','p','q','r','s','t','v','w','z','x'}; 

// probability of vowel
// ab ba aa bb
float coeff[] = {0.8, 0.2, 0.01, 0.9999};

// probability for beginning of the name
float begin[] = {0.4, 0.4, 0.1, 0.1};

char randomLetter(int type) {
	if(type == 0)
		return vowels[rand()%6];
	return consontants[rand()%20];
}

int weightedRandom() {
	float t = float(rand()) / RAND_MAX;
	float s = 0.0;
	for(int i=0; i<4; i++) {
		s += begin[i];
		if(t < s) return i;
	}
}

float Random() {
	return float(rand()) / RAND_MAX;
}

std::string generateName() {
	std::string name;
	int i;
	int n = rand() % 5 + 3;
	switch( weightedRandom() ) {
		case 0: name.push_back('a');
			name.push_back('b');
			break;
		case 1: name.push_back('b');
			name.push_back('a');
			break;
		case 2: name.push_back('a');
			name.push_back('b');
			break;
		case 3: name.push_back('b');
			name.push_back('b');
			break;
	}
	int l = 1;
	int c;
	while(l < n) {
		if(name[l-1] == 'a') {
			if(name[l] == 'b')
				c = 0;
			if(name[l] == 'a')
				c = 2;
		} else {
			if(name[l] == 'b')
				c = 3;
			if(name[l] == 'a')
				c = 1;
		}
		if(Random() < coeff[c])
			name.push_back('a');
		else
			name.push_back('b');
		l++;
	}
	for(i=0; i<n; i++)
		name[i] = randomLetter(name[i] == 'b');
	return name;
}	

void main() {
	ConsoleRenderer con;
	con.init();
	con.setTitle(L"Yo!");
	int i = 0;
	DWORD c = 0;
	while(c != VK_SPACE) {
		con.clear();
		for (unsigned int j = 0; j < 256; j++)
		{
			//con.setCell(j % 64, j / 64, j, WHITE);
			con.setAttr(j % 32, j / 32, WHITE);
			con.setAsciiChar(j % 32, j / 32, CHAR(j));
		}
		con.swap();
		c = con.getKeyPressed();
		i = (i + 1) % 60;
		Sleep(DELTATIME);
	}
}