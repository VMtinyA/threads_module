#include "threads_start.h"

//*****************************************************************************
// Функция, "инкапсулирующая" создание всех потоков в программе,
// в данной функции вызываются "функции старта" всех групп используемых потоков
unsigned char threads_start (void) {

    if (async_threads_start()) {
        printf("Async_thread error\n");
        return 1;
    }
    if (sync_threads_start()) {
        printf("Sync_thread error\n");
        return 1;
    }

    return 0;
}

