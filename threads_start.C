#include "threads_start.h"

pthread_t *sigReceivers;
//*****************************************************************************
// Функция, "инкапсулирующая" создание всех потоков в программе,
// в данной функции вызываются "функции старта" всех групп используемых потоков
void threads_start (void) {

    sigReceivers = (pthread_t *) calloc (SigReceiversNum, sizeof (pthread_t));
    *sigReceivers = async_threads_start();
    *(sigReceivers + 1) = sync_threads_start();

}

