#include "ConsoleVisualization.h"
#include "../config.h"
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

COORD startCoord;

CONSOLE_SCREEN_BUFFER_INFO getConsoleInfo() {
	HANDLE hStdHnd = GetStdHandle(STD_OUTPUT_HANDLE);	// указатель на действующую консолб
	CONSOLE_SCREEN_BUFFER_INFO buff;					// структура в которой будет информация в консоли
	GetConsoleScreenBufferInfo(hStdHnd, &buff);			// информацию о консоли

	return buff;
}

short getConsoleWidth(void) {
	return getConsoleInfo().srWindow.Right;
}

COORD getConsoleCursorCoord(void) {
	return getConsoleInfo().dwCursorPosition;
}

void ShowPointer(void) {
	size_t shift = (length_visible_part - 1) / 2;
	for (int i = 0; i < shift; i++)
		putchar(' ');

	putchar(pointer_label);

	for (int i = 0; i < shift; i++)
		putchar(' ');
}

void ShowTape(DL_Node* pTape) {
	size_t shift = (length_visible_part - 1) / 2;
	for (int i = 0; i < shift; i++)
		pTape = DL_Prev(pTape);

	putchar('\r'); // переход каретки в самое начало строки в консоли
	for (int i = 0; i < length_visible_part; i++) {
		if (pTape->date == 0) {
			putchar(empty_label);
		}
		else
		{
			putchar(filled_label);
		}
		pTape = DL_Next(pTape);
	}
}

void CS_Init(DL_Node* pTape)
{
	if (length_visible_part == 0) {
		length_visible_part = getConsoleWidth();
	}

	if (length_visible_part % 2 == 0) {
		length_visible_part -= 1;
	}

	startCoord = getConsoleCursorCoord();

	ShowPointer();

	putchar('\n');
	ShowTape(pTape);
}

void CS_Show(DL_Node* pTape)
{
	HANDLE hStdHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdHnd, startCoord);

	for (int i = 0; i < length_visible_part; i++) {
		putchar(' ');
	}
	putchar(' ');
	for (int i = 0; i < length_visible_part; i++) {
		putchar(' ');
	}
	SetConsoleCursorPosition(hStdHnd, startCoord);

	length_visible_part = getConsoleWidth();
	if (length_visible_part % 2 == 0) {
		length_visible_part -= 1;
	}
	ShowPointer();
	putchar('\n');
	ShowTape(pTape);
}
