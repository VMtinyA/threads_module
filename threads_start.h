#ifndef THREADS_START_H
#define THREADS_START_H

//************************************************************************************
/* Модуль содержит:
 * 1) функцию, "инкапсулирующую" создание всех потоков в программе;
*
 * В модуль должны быть включены объявления "инкапсулирующих" функций
 * из соответствующих модулей
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define SigReceiversNum      2

extern pthread_t *sigReceivers;

extern sem_t sem_SI2;
extern sem_t sem_ISA;

//************************************************************************************
/* Прототипы функций */

extern pthread_t async_threads_start (void);
extern pthread_t sync_threads_start (void);

// Функция, "инкапсулирующая" создание всех потоков в программе,
// в данной функции вызываются "функции старта" всех групп используемых потоков
extern void threads_start(void);

#endif // THREADS_START_H
