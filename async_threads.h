#ifndef ASYNC_THREADS_H
#define ASYNC_THREADS_H

/* Модуль предназначен для работы с асинхронным потоком в программе.
 * Асинхронный поток управляется сигналом прерывания по шине ISA
 * Включает в себя:
 * 1) необходимые константы (частоты, макс. число потоков, приоритет потока);
 * 2) глобальные переменные (семафор для потока);
 * 3) функция модуля аsync_thread_start() - "инкапсулирующая" функция создания потока,
 * должна вызываться в "родительском" модуле threads_start;
 * 4) функция async_thread() - обертка для вызова реальной функции.
 *
 * Модуль является "дочерним" по отношению к threads_start.
 */

#include "abstract_thread.h"

//****************************************************************************************************************
/* Константы */

#define ASYNC_THREADS_NUM          1               // макс. число используемых потоков
#define ASYNC_PRIORITY                   99               // макс. приоритет для стратегии планирования FIFO

//******************************************************************************
/* Глобальные переменные */

sem_t sem_ISA; // семафор для прерывания по ISA (выдает обработчик прерывания)

//****************************************************************************
/* Прототипы функций */

// обертка для вызова реальной функции
int async_thread (void);

// функция должна создать асинхронный поток
// эта функция вводится для "инкапсуляции" всего, что принадлежит асинхронным потокам, в данном модуле
// должна вызываться в "родительском" модуле "threads_start"
unsigned char async_threads_start(void);

#endif // ASYNC_THREADS_H
