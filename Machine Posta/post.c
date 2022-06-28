#define _CRT_SECURE_NO_WARNINGS
#include "post.h"
#include "DoublyLinkedListBool/DoublyLinkedListBool.h"
#include "SinglyLinkedListString/SinglyLinkedListString.h"
#include "ConsoleVisualization/ConsoleVisualization.h"
#include "config.h"
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

DL_Node* pTape; // указатель на ленту

// Сдвиг на ленте вправо
void RightShift() {
	pTape = DL_Next(pTape);
}

// Сдвиг на ленте влево
void LeftShift() {
	pTape = DL_Prev(pTape);
}

// Поставить метку на указывающее место в ленте
void PutLabel() {
	pTape->date = true;
}

// Убрать метку на указывающее место в ленте
void RemoveLabel() {
	pTape->date = false;
}

// Условие Если пустая, то N, иначе M
bool Condition() {
	return !(pTape->date);
}

// Выполнение комады. Возвращает следующую строчку
SL_Node* ExecuteCMD(SL_Node* cmd) {
	switch (cmd->cmd)
	{
	case '>':	// Сдвиг вправо
	{
		RightShift();
		return cmd->str1.point;
	}
	case '<':	// Сдвиг влево
	{
		LeftShift();
		return cmd->str1.point;
	}
	case '1':	// Поставить метку
	{
		PutLabel();
		return cmd->str1.point;
	}
	case '0':	// Убрать метку
	{
		RemoveLabel();
		return cmd->str1.point;
	}
	case '?':	// Условие
	{
		if (Condition() == true) {
			return cmd->str1.point;
		}
		else
		{
			return cmd->str2.point;
		}
	}
	case '!':	// Завершение скрипта
	{
		return NULL;
	}
	default:
		break;
	}
	return NULL;
}

void Interpreter(FILE* stream) {
	char cmd;

	while (fscanf(stream, "%c", &cmd) != EOF && cmd != 'S')
	{
		switch (cmd)
		{

		case '>':	// Сдвиг вправо
		case '<':	// Сдвиг влево
		case '1':	// Поставить метку
		case '0':	// Убрать метку
		{
			int numStr;
			fscanf(stream, "%i", &numStr);
			SL_AddBack(cmd, numStr, 0);
			fgetc(stream);
			break;
		}
		case '?':	// Условие
		{
			int numStr1, numStr2;
			fscanf(stream, "%i, %i", &numStr1, &numStr2);
			SL_AddBack(cmd, numStr1, numStr2);
			fgetc(stream);
			break;
		}
		case '!':	// Завершение скрипта
		{
			SL_AddBack(cmd, 0, 0);
			fgetc(stream);
			break;
		}
		case 'V':	// Перемешение указателя на ленту
		{
			int shift;
			fscanf(stream, "%i", &shift);
			if (shift > 0) {
				while (shift != 0)
				{
					RightShift();
					shift--;
				}
			}
			else if (shift < 0)
			{
				while (shift != 0)
				{
					LeftShift();
					shift++;
				}
			}
			fgetc(stream);
			break;
		}
		case 'P':	// Первоначальные настройки ленты
		{
			DL_Node* pTapeSave = pTape;
			char ch = fgetc(stream);
			while (ch != '\n')
			{

				if (ch == '0') {
					RemoveLabel();
					RightShift();
				}
				else if (ch == '1')
				{
					PutLabel();
					RightShift();
				}

				ch = fgetc(stream);
			}
			pTape = pTapeSave;
			break;
		}
		case 'S':	// Запуск скрипта
		{
			fgetc(stream);
			break;
		}
		default:
			break;
		}
	}
}

// Запуск Машины Поста с интерпертатором
void Post_Run(int argc, char* argv[])
{
	pTape = DL_Init(); // Инициализация ленты
	FILE* f = NULL;

	if (argc > 1) {
		f = fopen(argv[1], "r");
		if (f == NULL) {
			printf("ERROR: the file was not found.\n");
		}
	}

	if (f == NULL) {
		Interpreter(stdin);
	}
	else
	{
		Interpreter(f);
		fclose(f);
	}

	SL_Optimizat(); // Оптемизация списка строк
	SL_Node* listStr = SL_GetHead();
	
	CS_Init(pTape); // Инициализация визуала ленты
	size_t iteration = 0;
	while (listStr != NULL && 
		(iteration_limits == 0 || iteration < iteration_limits))
	{
		if (kbhit()) // Проверка на нажатие любой кнопки
			break;

		Sleep(delay);	// Пауза на delay миллисекунд 
		listStr = ExecuteCMD(listStr);
		CS_Show(pTape);

		iteration++;
	}
	putchar('\n');
	DL_Print(); // Печать данных с ленты

	SL_Destroy();
	DL_Destroy();
}
