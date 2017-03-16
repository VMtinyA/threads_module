//*************************************************************************************************
// Описание модуля в файле заголовка

#include "async_threads.h"

sem_t sem_ISA;

//*************************************************************************************************
// функция должна создать асинхронный поток
// эта функция вводится для "инкапсуляции" всего, что принадлежит асинхронным потокам, в данном модуле
// должна вызываться в модуле "threads_start"
pthread_t async_threads_start (void) {

    // инициализация потока, работающего по сигналу прерывания c шины ISA
    // инициализация пускового семафора потока - поток приостановлен
     if (sem_init(&sem_ISA, 1, 0)) {
         perror("Can't init sem_ISA");
     }

     // В программе 1 асинхронный поток - опрашивает ДЛУ по сигналу прерывания по шине ISA
     pthread_t ISA_thread_id;
     pthread_attr_t ISA_thread_attr;
     THREAD_ARG ISA_thread_args;

     strncpy(ISA_thread_args.name, "ISA_thread", strlen("ISA_thread") + 1);
     ISA_thread_args.func = ISA_thread;
     ISA_thread_args.isComplete = 1;
     ISA_thread_args.priority = ISA_THREAD_PRIORITY;

     create_thread(&(ISA_thread_id), &(ISA_thread_attr), &(ISA_thread_args));

     return ISA_thread_id;
}

//******************************************************************************
// обертка для вызова реальной функции

int ISA_thread (void) {

    while (1) {

        // попытка захвата семафора
        if (sem_wait(&sem_ISA)) {
            perror("sem_ISA error");
            return 1;
        }
        // вызов реальной функции
        printf("t_async\n");

    }
    // есть возможность расширения - устанавливать разный код возврата в isComplete
    // для его дальнейшей проверки

    return 0;
}
