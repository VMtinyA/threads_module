//*************************************************************************************************
// Описание модуля в файле заголовка

#include "async_threads.h"

//*************************************************************************************************
// функция должна создать асинхронный поток
// эта функция вводится для "инкапсуляции" всего, что принадлежит асинхронным потокам, в данном модуле
// должна вызываться в "родительском" модуле "threads_start"
unsigned char async_threads_start (void) {

    // инициализация потока, работающего по сигналу прерывания c шины ISA
    // инициализация пускового семафора потока - поток приостановлен
     sem_init(&sem_ISA, 1, 0);
     //
     int err;
     pthread_attr_t attr;
     pthread_t thread;
     THREAD_ARG args = {1, ASYNC_PRIORITY};
     struct sched_param schedparam;  // параметры планирования (приоритет)

     // инициализация описателя атрибутов потока управления стандартными значениями атрибутов
     if (err = pthread_attr_init(&attr)) {
         perror("Attribute creation failed");
         return 0;
     }

    // установка атрибута "inheritsched" (стратегия планирования и связанные с ней атрибуты не наследуются - берутся из attr)

    if (err = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) {
        perror("Setting inheritsched failed");
        return 0;
    }

    // установка атрибута "schedpolicy" (создаваемый поток будет иметь стратегию планирования SCHED_FIFO)

    if (err = pthread_attr_setschedpolicy(&attr, SCHED_FIFO)) {
        perror("Setting scheduling policy failed");
        return 0;
    }

    // создаваемый поток будет иметь приоритет, заданный в US_ARG
    schedparam.sched_priority = args.priority;
    if (err = pthread_attr_setschedparam(&attr, &schedparam)) {
        perror("Setting schedparam failed");
        return 0;
    }

    if (err = pthread_create(&thread, &attr, &async_thread, &args)) {
        perror("Cant create manager_thread\n");
        return 0;
    }

     pthread_attr_destroy(&attr);
     return 1;
}

//******************************************************************************
// обертка для вызова реальной функции

static void *async_thread (void *param) {

    THREAD_ARG *arg = (THREAD_ARG *) param;
    arg->isComplete = 0;
    while (1) {

        // попытка захвата семафора
        sem_wait(&sem_ISA);
        // вызов реальной функции
        printf("t_async\n");

    }
    arg->isComplete = 1;
    // есть возможность расширения - устанавливать разный код возврата в isComplete
    // для его дальнейшей проверки

    return NULL;
}
