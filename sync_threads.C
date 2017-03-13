//*************************************************************************************************
// Описание модуля в файле заголовка

#include "sync_threads.h"

sem_t sem_SI2;

//*************************************************************************************************
// функция должна создать поток - диспетчер для управления синхронными потоками f0, f1, f2
// эта функция вводится для инкапсуляции всего, что принадлежит синхронным потокам, в данном модуле

unsigned char sync_threads_start (void) {

    // инициализация потока-диспетчера, работающего по сигналу прерывания СИ2
    // инициализация пускового семафора потока semSI2 - поток-диспетчер приостановлен
     sem_init(&sem_SI2, 1, 0);

     pthread_t manager_thread;
     THREAD_ARG manager_args;

     // Заполнение структуры параметров потока - диспетчера
     manager_args.func = &thread_manager;
     manager_args.isComplete = 1;
     manager_args.priority = MANAGER_PRIORITY;

     // функция из "родительского" модуля threads_start
     if (set_thread_attr(&manager_args))
         return 1;

     if (pthread_create(&manager_thread,
                                         &(manager_args.attr),
                                         start_thread,
                                         (void *) (&manager_args))) {
        perror("Can't create manager_thread\n");
        return 1;
     }
     printf("Manager of sync threads created.\n");

     if (pthread_attr_destroy(&(manager_args.attr))) {
        perror("Attribute destruction failed");
        return 1;
     }

    return 0;
}

//****************************************************************************************************************
// функция-диспетчер циклических потоков
// при вызове должна создать все потоки и управлять ими
void *thread_manager(void *param) {

    // Массив "делителей" сигнала используется для вызова всех потоков,
    // работающих по делителю сигнала СИ2 (всех, кроме 0, он создается всегда)
    short thread_count[SYNC_THREADS_NUM];

    pthread_t slave_threads[SYNC_THREADS_NUM];
    // Массив структур параметров дочерних потоков
    THREAD_ARG slave_args[SYNC_THREADS_NUM];

    // Массив указателей на функции дочерних потоков
    thread_func func_array[SYNC_THREADS_NUM] = {
        func0,
        func1,
        func2
        };

    //заполнение массива структур параметров дочерних потоков
    for (unsigned short i = 0; i < SYNC_THREADS_NUM; ++i) {
        // функция потока берется из массива указателей
        slave_args[i].func = func_array[i];
        // каждый дочерний поток "завершен" и готов к работе
        slave_args[i].isComplete = 1;
         // приоритет каждого из дочерних на 1 меньше предыдущего
        slave_args[i].priority = MANAGER_PRIORITY - (i + 1);
        // инициализация атрибутов всех дочерних потоков (ф-ия из модуля threads_start)
        if (set_thread_attr(&slave_args[i]))
            return NULL;
        // при первом запуске создаются все потоки
        thread_count[i] = 0;
    }

    while (1) {

        // попытка захвата семафора semSI2 (должен быть выдан обработчиком прерывания SI2)
        sem_wait(&sem_SI2);

        /* Управление потоками */

        // Управление синхронными потоками осуществляется по принципу "потока на соединение".
        // Когда поток нужен - он создается (согласно частоте срабатывания)
        // При получении сигнала СИ2 диспетчер проверяет счетчик каждого потока и сравнивает его с делителем
        for (unsigned short i = 0; i < SYNC_THREADS_NUM; ++i) {
            // Если счетчик не 0 - уменьшить счетчик
            if (thread_count[i])
                --thread_count[i];
            // Если 0 - обновить счетчик до значения делителя и создать соответств. поток
            // (для thread0 делитель всегда 0, т.к. этот поток создается каждый раз при получении СИ2)
            else {
                thread_count[i] = thread_dividers[i];
                // если предыдущий такой же поток закончил работу
                if (slave_args[i].isComplete) {
                    if (pthread_create(&slave_threads[i],
                                                        &(slave_args[i].attr),
                                                        start_thread,
                                                        (void *) (&slave_args[i]))) {
                       fprintf(stderr, "Can't create %u thread\n", i);
                       return NULL;
                    }
                }
                else {
                    fprintf(stderr, "Thread %u isnt complete", i);
                }
            }
        } // end of for

    } // end of while
    return NULL;
}

//****************************************************************************************************************
// функция потока 0 (1000 Гц)
int func0(void) {

        /* THREAD_ARG *arg = (THREAD_ARG *) param;
        arg->isComplete = 0; */

        // вызов реальной функции
        printf("t0\n");

        //arg->isComplete = 1;
        // есть возможность расширения - устанавливать разный код возврата в isComplete
        // для его дальнейшей проверки в диспетчере
        return 0;
}

//****************************************************************************************************************
// функция потока 1 (100/200 Гц)
int func1(void) {

    /* THREAD_ARG *arg = (THREAD_ARG *) param;
    arg->isComplete = 0; */

    // вызов реальной функции
    printf("t1\n");

    //arg->isComplete = 1;
    return 0;
}

//****************************************************************************************************************
// функция потока 2 (1/10 Гц)
int func2(void) {

    /*THREAD_ARG *arg = (THREAD_ARG *) param;
    arg->isComplete = 0; */

    // вызов реальной функции
    printf("t2\n");

    //arg->isComplete = 1;
    return 0;
}
