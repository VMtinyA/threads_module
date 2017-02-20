#ifndef SYNC_THREADS_H
#define SYNC_THREADS_H

/* Модуль предназначен для работы с синхронными потоками в программе.
 * Синхронные потоки управляются потоком - диспетчером, работающим по сигналу прерывания СИ2
 * Включает в себя:
 * 1) необходимые константы (частоты, макс. число потоков, приоритет потока - диспетчера);
 * 2) глобальные переменные (семафор для потока-диспетчера);
 * 3) функция модуля sync_threads_start() - "инкапсулирующая" функция создания потока - диспетчера,
 * должна вызываться в "родительском" модуле threads_start;
 * 4) функция thread_manager() создает все дочерние потоки и управляет их вызовом;
 * 5) функции потоков func[i] - обертки для вызова реальных функций.
 *
 * Модуль является "дочерним" по отношению к threads_start.
 */

#include "threads_start.h"

//****************************************************************************************************************
/* Константы */

// Частоты синхронных потоков
#define Frequency0          1000
#define Frequency1          200
#define Frequency2          500

#define SYNC_THREADS_NUM          3      // макс. число используемых потоков
#define MANAGER_PRIORITY         98     // макс. приоритет для стратегии планирования FIFO - 1
// приоритет асинхронного потока выше

//******************************************************************************
/* Глобальные переменные */

sem_t sem_SI2; // семафор для прерывания по СИ2 (выдает обработчик прерывания)

//****************************************************************************
/* Прототипы функций */

/* Функции дочерних потоков */
// служат "обертками" для вызова реальных функций
static void *func0 (void *param);
static void *func1 (void *param);
static void *func2 (void *param);

// функция-диспетчер циклических потоков
// при вызове должна создать все потоки и управлять ими
static void *thread_manager (void *param);

// функция должна создать поток - диспетчер для управления синхронными потоками f0, f1, f2
// эта функция вводится для "инкапсуляции" всего, что принадлежит синхронным потокам, в данном модуле
// должна вызываться в "родительском" модуле "threads_start"
unsigned char sync_threads_start(void);

#endif // SYNC_THREADS_H