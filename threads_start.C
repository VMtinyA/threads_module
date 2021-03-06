//************************************************************************************
/* Модуль содержит:
 * 1) функцию, "инкапсулирующую" создание всех потоков в программе;
*
 * В модуль должны быть включены объявления "инкапсулирующих" функций
 * из соответствующих модулей
 */
/* Модуль предназначен для работы с асинхронным потоком в программе.
 * Асинхронный поток управляется сигналом прерывания по шине ISA
 * Включает в себя:
 * 1) необходимые константы (частоты, макс. число потоков, приоритет потока);
 * 2) глобальные переменные (семафор для потока);
 * 3) функция модуля аsync_thread_start() - "инкапсулирующая" функция создания потока,
 * должна вызываться в модуле threads_start;
 * 4) функция ISA_thread() - обертка для вызова реальной функции.
 *
 * Модуль является "дочерним" по отношению к abstract_thread.
 */
/* Модуль предназначен для работы с синхронными потоками в программе.
 * Синхронные потоки управляются потоком - диспетчером, работающим по сигналу прерывания СИ2
 * Включает в себя:
 * 1) необходимые константы (частоты, макс. число потоков, приоритет потока - диспетчера);
 * 2) глобальные переменные (семафор для потока-диспетчера);
 * 3) функция модуля sync_threads_start() - "инкапсулирующая" функция создания главного потока,
 * должна вызываться в модуле threads_start;
 * 4) функция sync_master() является функцией главного потока
 * (опрашивающего КЛГ и обрабатывающего показания ДУС), синхронно создает дочерний поток slave_thread;
 * 5) функция потока slave_thread() - обертка для вызова функций с навигационными алгоритмами.
 *
 * Модуль является "дочерним" по отношению к abstract_thread.
 */

#include "abstract_thread.h"

//****************************************************************************************************************
/* Константы */

// Число потоков в программе
#define THREADS_NUM                      3

// Частоты потоков
#define Frequency_ISA                       1750
#define Frequency_SI2_master          1000
#define Frequency_SI2_slave             200

// Приоритет потоков
#define ISA_THREAD_PRIORITY             MAX_PRIORITY_FIFO
#define SYNC_MASTER_PRIORITY          MAX_PRIORITY_FIFO - 1
#define SYNC_SLAVE_PRIORITY             MAX_PRIORITY_FIFO - 2
// приоритет асинхронного потока наивысший

//******************************************************************************
/* Глобальные переменные */

sem_t sem_ISA; // семафор для прерывания по ISA (выдает обработчик прерывания)
sem_t sem_SI2; // семафор для прерывания по СИ2 (выдает обработчик прерывания)
sem_t sem_slave; // семафор для контроля дочернего потока

// указатель на массив структур потоков, получающих сигналы от обработчика прерываний
THREAD_ARG *threads_arguments;

//************************************************************************************
/* Прототипы функций */

// Функция, "инкапсулирующая" создание всех потоков в программе,
// для вызова из main
extern unsigned char threads_start(void);

// Функция потока для обработки прерывания по шине ISA
static int ISA_thread (void);

// Функция главного потока (опрашивающего КЛГ и обрабатывающего показания ДУС),
// синхронно создает дочерний поток slave_thread при необходимости
static int sync_master (void);
/* Функция дочернего потока */
static int sync_slave (void);

//*****************************************************************************
/* Реализация функций */

// Функция, "инкапсулирующая" создание всех потоков в программе,
// в данной функции вызываются "функции старта" всех групп используемых потоков
unsigned char threads_start (void) {

    threads_arguments = (THREAD_ARG *) calloc (THREADS_NUM, sizeof (THREAD_ARG));
    THREAD_ARG *rcvrs_iterator;
    int ret;
    pthread_attr_t thread_attr;

    // инициализация потока, работающего по сигналу прерывания c шины ISA
    // инициализация пускового семафора потока - поток приостановлен
    ret = sem_init(&sem_ISA, 1, 0);
     if (ret == -1) {
         fprintf(stdout, "Can't init sem_ISA\n");
         return 1;
     }

     // В программе один асинхронный поток - опрашивает ДЛУ по сигналу прерывания по шине ISA
     // Заполнение его структуры
     rcvrs_iterator = threads_arguments;
     strncpy(rcvrs_iterator->name, "ISA_thread", strlen("ISA_thread") + 1);
     rcvrs_iterator->thread_id = 0;
     rcvrs_iterator->func = ISA_thread;
     rcvrs_iterator->priority = ISA_THREAD_PRIORITY;
     create_thread(&thread_attr, rcvrs_iterator);

     // инициализация потока, работающего по сигналу прерывания СИ2
     // инициализация пускового семафора потока - поток приостановлен
     ret = sem_init(&sem_SI2, 1, 0);
     if (ret == -1) {
         fprintf(stdout, "Can't init sem_SI2\n");
         return 1;
     }

     // Заполнение структуры параметров главного синхронного потока
     rcvrs_iterator += 1;
     strncpy(rcvrs_iterator->name, "Sync_master", strlen("Sync_master") + 1);
     rcvrs_iterator->thread_id = 0;
     rcvrs_iterator->func = sync_master;
     rcvrs_iterator->priority = SYNC_MASTER_PRIORITY;
     create_thread(&thread_attr, rcvrs_iterator);

     // Заполнение структуры параметров дочернего синхронного потока
     rcvrs_iterator += 1;
     strncpy(rcvrs_iterator->name, "Sync_slave", strlen("Sync_slave") + 1);
     rcvrs_iterator->thread_id = 0;
     rcvrs_iterator->func = sync_slave;
     rcvrs_iterator->priority = SYNC_SLAVE_PRIORITY;
     create_thread(&thread_attr, rcvrs_iterator);

     return 0;
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

//****************************************************************************************************************
// Функция главного потока (опрашивающего КЛГ и обрабатывающего показания ДУС),
// синхронно создает дочерний поток slave_thread при необходимост
int sync_master (void) {

    unsigned short thread_divider = 0;
    int ret;

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
            thread_divider = Frequency_SI2_master / Frequency_SI2_slave - 1;
            ret = sem_post(&sem_slave);
            if (ret == -1) {
                fprintf(stdout, "sem_slave error\n");
                return 1;
            }
        }

    } // end of while
    return 0;
}

//****************************************************************************************************************
// функция дочернего потока (200 Гц)
int sync_slave (void) {

        int ret;
        while (1) {
            ret = sem_wait(&sem_slave);
            if (ret == -1) {
                fprintf(stdout, "sem_slave error\n");
                return 1;
            }
            // вызов реальной функции
            printf("t_slave\n");
        }


        // есть возможность расширения - устанавливать разный код возврата в isComplete
        // для его дальнейшей проверки
        return 0;
}
