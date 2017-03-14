//*************************************************************************************************
// Описание модуля в файле заголовка

#include "async_threads.h"

sem_t sem_ISA;

//*************************************************************************************************
// функция должна создать асинхронный поток
// эта функция вводится для "инкапсуляции" всего, что принадлежит асинхронным потокам, в данном модуле
// должна вызываться в модуле "threads_start"
unsigned char async_threads_start (void) {

    // инициализация потока, работающего по сигналу прерывания c шины ISA
    // инициализация пускового семафора потока - поток приостановлен
     if (sem_init(&sem_ISA, 1, 0)) {
         perror("Can't init sem_ISA");
         return 1;
     }

     pthread_t async_thread_id;
     pthread_attr_t async_attr;
     THREAD_ARG async_args;

     strncpy(async_args.name, "async_thread", strlen("async_thread") + 1);
     async_args.func = async_thread;
     async_args.isComplete = 1;
     async_args.priority = ASYNC_PRIORITY;


     if (create_thread(&(async_thread_id), &(async_attr), &(async_args)))
             return 1;
     return 0;
}

//******************************************************************************
// обертка для вызова реальной функции

int async_thread (void) {

    /* THREAD_ARG *arg = (THREAD_ARG *) param;
    arg->isComplete = 0; */
    while (1) {

        // попытка захвата семафора
        if (sem_wait(&sem_ISA)) {
            perror("sem_ISA error");
            return 1;
        }
        // вызов реальной функции
        printf("t_async\n");

    }
    //arg->isComplete = 1;
    // есть возможность расширения - устанавливать разный код возврата в isComplete
    // для его дальнейшей проверки

    return 0;
}
