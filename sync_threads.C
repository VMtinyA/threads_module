//*************************************************************************************************
// Описание модуля в файле заголовка

#include "sync_threads.h"

sem_t sem_SI2;

//*************************************************************************************************
// функция должна создать главный из синхронных потоков
// эта функция вводится для "инкапсуляции" в данном модуле всего, что принадлежит синхронным потокам
// должна вызываться в модуле "threads_start"

pthread_t sync_threads_start (void) {

     if (sem_init(&sem_SI2, 1, 0)) {
         perror("Can't init sem_SI2");
     }

     pthread_t master_id;
     pthread_attr_t master_attr;
     THREAD_ARG master_args;

     // Заполнение структуры параметров главного потока
     strncpy(master_args.name, "sync_master_thread", strlen("sync_master_thread") + 1);
     master_args.func = sync_master;
     master_args.isComplete = 1;
     master_args.priority = SYNC_MASTER_PRIORITY;

     // функция из "родительского" модуля abstract_thread
     create_thread(&(master_id),
                                &(master_attr),
                                &(master_args));

    return master_id;
}

//****************************************************************************************************************
// Функция главного потока (опрашивающего КЛГ и обрабатывающего показания ДУС),
// синхронно создает дочерний поток slave_thread при необходимост
int sync_master (void) {

    pthread_t slave_id;
    pthread_attr_t slave_attr;

    THREAD_ARG slave_args;
    // Заполнение структуры параметров дочернего потока
    strncpy(slave_args.name, "sync_slave_thread", strlen("sync_slave_thread") + 1);
    slave_args.func = slave_thread;
    slave_args.isComplete = 1;
    slave_args.priority = SYNC_SLAVE_PRIORITY;

    unsigned short thread_divider = 0;

    while (1) {

        // попытка захвата семафора semSI2 (должен быть выдан обработчиком прерывания SI2)
        if (sem_wait(&sem_SI2)) {
            perror("sem_SI2 error");
            return 1;
        }

        /* Вызов основных функций потока
         * ask_KLG();                       // Опрос КЛГ
         * procIndication_DLU();     // Обработка показаний ДЛУ
         *
         * пока здесь заглушка
         */
        printf("t_master\n");


        /* Управление потоками */

        // Управление дочерним потоком осуществляется по принципу "потока на соединение".
        // Когда поток нужен - он создается (согласно частоте срабатывания по делителю СИ2)
        if (thread_divider)
            --thread_divider;
        else {
            thread_divider = Frequency0 / Frequency1 - 1;
            if (create_thread(&(slave_id),
                                       &(slave_attr),
                                       &(slave_args)))
                    return 1;
        }

    } // end of while
    return 0;
}

//****************************************************************************************************************
// функция дочернего потока (200 Гц)
int slave_thread (void) {

        // вызов реальной функции
        printf("t_slave\n");

        // есть возможность расширения - устанавливать разный код возврата в isComplete
        // для его дальнейшей проверки
        return 0;
}

