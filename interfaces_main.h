#ifndef INTERFACES_MAIN_H
#define INTERFACES_MAIN_H

//*****************************************************************************
/* Заголовочный файл предназначен для включения в основной поток управления.
 * Основной поток вызывает только интерфейсные функции каждого модуля.
 *
 * Заголовочный файл содержит:
 * 1) описания глобальных переменных программы;
 * 2) описания "интерфейсных" функций, вызываемых в main();
 * 3) заголовки, включаемые в основной поток управления, в т.ч. стандартные
 *
 * Данные разбиты помодульно (п.3 отдельно).
 *
 */

//*****************************************************************************
/* Заголовки, включаемые в основной поток управления */
// Стандартные библиотеки С и POSIX
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Пользовательские

//*****************************************************************************
/* Модуль threads_start */

extern sem_t sem_SI2;
extern sem_t sem_ISA;

extern unsigned char threads_start(void);

//*****************************************************************************
/* Модуль fake_signal_emitter */

#endif // INTERFACES_MAIN_H
