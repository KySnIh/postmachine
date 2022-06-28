#pragma once
#ifndef _CV_HEADER
#define _CV_HEADER

#include "../DoublyLinkedListBool/DoublyLinkedListBool.h"

// Инициализация визуала ленты
void CS_Init(DL_Node* pTape);

// Выводит нанешное положение ленты
void CS_Show(DL_Node* pTape);

#endif // !_CV_HEADER