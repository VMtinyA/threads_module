//*************************************************************************************************
// Описание модуля в файле заголовка

#include "async_threads.h"

sem_t sem_ISA;

//*************************************************************************************************
// функция должна создать асинхронный поток
// эта функция вводится для "инкапсуляции" всего, что принадлежит асинхронным потокам, в данном модуле
// должна вызываться в "родительском" модуле "threads_start"
unsigned char async_threads_start (void) {

    // инициализация потока, работающего по сигналу прерывания c шины ISA
    // инициализация пускового семафора потока - поток приостановлен
     if (sem_init(&sem_ISA, 1, 0)) {
         perror("Can't init sem_ISA\n");
         return 1;
     }

     pthread_t thread;
     THREAD_ARG args = {async_thread, 1, ASYNC_PRIORITY};

     if (set_thread_attr(&args)) {
         perror("Setting async thread's attributes failed\n");
         return 1;
     }

     if (pthread_create(&thread,
                                         &(args.attr),
                                         start_thread,
                                         (void *) (&args))) {
        perror("Can't create async_thread\n");
        return 1;
     }

      if (pthread_attr_destroy(&(args.attr))) {
         perror("Attribute destruction failed\n");
         return 1;
      }

     return 0;
}

//******************************************************************************
// обертка для вызова реальной функции

int async_thread (void) {

    /* THREAD_ARG *arg = (THREAD_ARG *) param;
    arg->isComplete = 0; */
    while (1) {

        // попытка захвата семафора
        sem_wait(&sem_ISA);
        // вызов реальной функции
        printf("t_async\n");

    }
    //arg->isComplete = 1;
    // есть возможность расширения - устанавливать разный код возврата в isComplete
    // для его дальнейшей проверки

    return 0;
}
