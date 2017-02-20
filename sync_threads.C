//*************************************************************************************************
// Описание модуля в файле заголовка

#include "sync_threads.h"

//*************************************************************************************************
// функция должна создать поток - диспетчер для управления синхронными потоками f0, f1, f2
// эта функция вводится для инкапсуляции всего, что принадлежит синхронным потокам, в данном модуле

unsigned char sync_threads_start (void) {

    // инициализация потока-диспетчера, работающего по сигналу прерывания СИ2
    // инициализация пускового семафора потока semSI2 - поток-диспетчер приостановлен
     sem_init(&sem_SI2, 1, 0);

     int err;
     pthread_attr_t manager_attr;
     pthread_t manager_thread;
     THREAD_ARG manager_args = {1, MANAGER_PRIORITY};
     struct sched_param schedparam;  // параметры планирования (приоритет)

     // инициализация описателя атрибутов потока управления стандартными значениями атрибутов
     if (err = pthread_attr_init(&manager_attr)) {
         perror("Attribute creation failed");
         return 0;
     }

    // установка атрибута "inheritsched" (стратегия планирования и связанные с ней атрибуты не наследуются - берутся из attr)

    if (err = pthread_attr_setinheritsched(&manager_attr, PTHREAD_EXPLICIT_SCHED)) {
        perror("Setting inheritsched failed");
        return 0;
    }

    // установка атрибута "schedpolicy" (создаваемый поток будет иметь стратегию планирования SCHED_FIFO)

    if (err = pthread_attr_setschedpolicy(&manager_attr, SCHED_FIFO)) {
        perror("Setting scheduling policy failed");
        return 0;
    }

    // создаваемый поток будет иметь приоритет, заданный в US_ARG
    schedparam.sched_priority = manager_args.priority;
    if (err = pthread_attr_setschedparam(&manager_attr, &schedparam)) {
        perror("Setting schedparam failed");
        return 0;
    }

    if (err = pthread_create(&manager_thread, &manager_attr, &thread_manager, &manager_args)) {
        perror("Cant create manager_thread\n");
        return 0;
    }
    pthread_attr_destroy(&manager_attr);
    return 1;
}

//****************************************************************************************************************
// функция-диспетчер циклических потоков
// при вызове должна создать все потоки и управлять ими
static void *thread_manager(void *param) {

    unsigned short thread_1_divider = 0;
    unsigned short thread_2_divider = 0;
    int err;

    pthread_attr_t attr;
    pthread_t slave_threads[SYNC_THREADS_NUM];
    THREAD_ARG slave_args[SYNC_THREADS_NUM] = {
        {1, MANAGER_PRIORITY - 1},
        {1, MANAGER_PRIORITY - 2},
        {1, MANAGER_PRIORITY - 3}
    };

    struct sched_param schedparam;  // параметры планирования (приоритет)

    // инициализация описателя атрибутов потока управления стандартными значениями атрибутов
    if (err = pthread_attr_init(&attr))
        perror("Attribute creation failed");

   // установка атрибута "inheritsched" (стратегия планирования и связанные с ней атрибуты не наследуются - берутся из attr)

   if(err = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED))
       perror("Setting inheritsched failed");

   // установка атрибута "schedpolicy" (создаваемый поток будет иметь стратегию планирования SCHED_FIFO)

   if(err = pthread_attr_setschedpolicy(&attr, SCHED_FIFO))
       perror("Setting scheduling policy failed");

    while (1) {

        // попытка захвата семафора semSI2 (должен быть выдан обработчиком прерывания SI2)
        sem_wait(&sem_SI2);

        /* Управление потоками */

        // Поток 0 создается всегда при получении прерывания
        // если предыдущий такой же поток закончил работу
        // есть возможность расширения - проверять isComplete с помощью case
        if (slave_args[0].isComplete) {
            // создаваемый поток будет иметь приоритет, заданный в THREADS_ARG
            schedparam.sched_priority = slave_args[0].priority;
            if (err = pthread_attr_setschedparam(&attr, &schedparam))
                printf("Setting schedparam of 0 thread failed");
            if (err = pthread_create(&slave_threads[0], &attr, &func0, (&slave_args[0])))
                printf("Cant create 0 thread");
        }
        else {
            printf("Thread 0 isnt complete");
        }

        // Поток 1 создается с помощью счетчика thread_1_divider
        if (thread_1_divider) {
            --thread_1_divider;
        } else {
            thread_1_divider = Frequency0 / Frequency1 - 1;
            // если предыдущий такой же поток закончил работу
            if (slave_args[1].isComplete) {
                // создаваемый поток будет иметь приоритет, заданный в THREADS_ARG
                schedparam.sched_priority = slave_args[1].priority;
                if (err = pthread_attr_setschedparam(&attr, &schedparam))
                    printf("Setting schedparam of 1 thread failed");
                if (err = pthread_create(&slave_threads[1], &attr, &func1, (&slave_args[1])))
                    printf("Cant create 1 thread");
            }
            else {
                printf("Thread 1 isnt complete");
            }
        }

        // Поток 2 создается с помощью счетчика thread_2_divider
        if (thread_2_divider) {
            --thread_2_divider;
        } else {
            thread_2_divider = Frequency0 / Frequency2 - 1;
            // если предыдущий такой же поток закончил работу
            if (slave_args[2].isComplete) {
                // создаваемый поток будет иметь приоритет, заданный в THREADS_ARG
                schedparam.sched_priority = slave_args[2].priority;
                if (err = pthread_attr_setschedparam(&attr, &schedparam))
                    printf("Setting schedparam of 2 thread failed");
                if (err = pthread_create(&slave_threads[2], &attr, &func2, (&slave_args[2])))
                    printf("Cant create 2 thread");
            }
            else {
                printf("Thread 2 isnt complete");
            }
        }
    }
    return NULL;
}

//****************************************************************************************************************
// функция потока 0 (1000 Гц)
static void *func0 (void *param) {

        THREAD_ARG *arg = (THREAD_ARG *) param;
        arg->isComplete = 0;
        // вызов реальной функции
        printf("t0\n");
        arg->isComplete = 1;
        // есть возможность расширения - устанавливать разный код возврата в isComplete
        // для его дальнейшей проверки в диспетчере
        return NULL;
}

//****************************************************************************************************************
// функция потока 1 (100/200 Гц)
static void *func1(void *param) {

    THREAD_ARG *arg = (THREAD_ARG *) param;
    arg->isComplete = 0;
    // вызов реальной функции
    printf("t1\n");
    arg->isComplete = 1;
    return NULL;
}

//****************************************************************************************************************
// функция потока 2 (1/10 Гц)
static void *func2 (void *param) {

    THREAD_ARG *arg = (THREAD_ARG *) param;
    arg->isComplete = 0;
    // вызов реальной функции
    printf("t2\n");
    arg->isComplete = 1;
    return NULL;
}
