#ifndef THREADS_START_H
#define THREADS_START_H

/* Модуль содержит:
 * 1) функцию, "инкапсулирующую" создание всех потоков в программе;
 * 2) структуру для работы с потоками;
 * 3) общие заголовочные файлы для модулей потоков.
 * Модуль должен быть включен во все модули, связанные с созданием и обслуванием потоков
 * В модуль должны быть включены заголовочные файлы "дочерних" модулей потоков.
 */

//************************************************************************************
/* Заголовочные файлы */

// Стандартные библиотеки С и POSIX
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#include <stdio.h>

// Заголовочные файлы модулей для работы с группами потоков
#include "sync_threads.h"           // синхронные потоки
#include "async_threads.h"        // асинхронные потоки

//************************************************************************************
/* Параметры потоков и глобальные переменные */

/* параметры потоков управления */
typedef struct {
       unsigned short isComplete;       // флаг для проверки завершения потока
       int priority;                                      // значение приоритета (1 - 99 для стратегии FIFO)
       //struct data;                                  // структура для данных,
                                                                   // передаваемых в реальную функцию потока
} THREAD_ARG;

//************************************************************************************
/* Прототипы функций */

// Функция, "инкапсулирующая" создание всех потоков в программе,
// в данной функции вызываются "функции старта" всех групп используемых потоков
unsigned char threads_start(void);

#endif // THREADS_START_H
