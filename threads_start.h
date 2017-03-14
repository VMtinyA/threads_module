#ifndef THREADS_START_H
#define THREADS_START_H

/* Модуль содержит:
 * 1) функцию, "инкапсулирующую" создание всех потоков в программе;
*
 * В модуль должны быть включены объявления "инкапсулирующих" функций
 * из соответствующих модулей
 */

#include <stdio.h>
#include <semaphore.h>

extern sem_t sem_SI2;
extern sem_t sem_ISA;

//************************************************************************************
/* Прототипы функций */

extern unsigned char async_threads_start(void);
extern unsigned char sync_threads_start(void);

// Функция, "инкапсулирующая" создание всех потоков в программе,
// в данной функции вызываются "функции старта" всех групп используемых потоков
extern unsigned char threads_start(void);


#endif // THREADS_START_H
