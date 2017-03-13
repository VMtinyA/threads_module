#include "threads_start.h"

//*****************************************************************************
// Функция, "инкапсулирующая" создание всех потоков в программе,
// в данной функции вызываются "функции старта" всех групп используемых потоков
unsigned char threads_start (void) {

    if (async_threads_start()) {
        perror("Can't create async_thread");
        return 1;
    }
    if (sync_threads_start()) {
        perror("Can't create sync_threads");
        return 1;
    }

    return 0;
}

