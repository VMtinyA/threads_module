#ifndef ABSTRACT_THREAD_H
#define ABSTRACT_THREAD_H

/* Модуль содержит:
 * 1) структуру для работы с потоками;
 * 2) общие заголовочные файлы для модулей потоков;
 * 3) функции для создания и конфигурации параметров любых потоков.
 *
 * Модуль должен быть включен во все модули, связанные с созданием и обслуживанием потоков
 */

//************************************************************************************
/* Заголовочные файлы */

// Стандартные библиотеки С и POSIX
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define MAX_PRIORITY_FIFO           99 // максимальный приоритет при стратегии FIFO
#define FrequencySI2                     1000
#define FrequencyISA                     1750

//************************************************************************************
/* Параметры потоков и глобальные переменные */

/* Тип данных - указатель на функцию */
typedef int (*thread_func) (void);

/* Параметры потоков управления */
typedef struct {
       char name[30];                            // имя потока
       thread_func func;                         // указатель на исполняемую функцию потока
       unsigned char isComplete;          // флаг для проверки завершения потока
       unsigned short priority;               // значение приоритета (1 - 99 для стратегии FIFO)
       //struct data;                                // структура для данных,
                                                           // передаваемых в реальную функцию потока
} THREAD_ARG;

//************************************************************************************
/* Прототипы функций */

// Функция, заполняющая атрибуты потока
// для вызова из create_thread()
extern unsigned char set_thread_attr(pthread_attr_t *attr, unsigned short priority);

// Функция для вызова в pthread_create()
// вызывает функцию создаваемого потока из THREAD_ARG
extern void *start_thread (void *argv);

// Функция, создающая поток по структуре параметров потока
// для вызова из "дочерних" модулей
extern unsigned char create_thread (pthread_t *thread_id,
                                                           pthread_attr_t *attr,
                                                           THREAD_ARG *arg);

#endif // ABSTRACT_THREAD_H
