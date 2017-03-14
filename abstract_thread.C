#include "abstract_thread.h"

//*****************************************************************************
// Функция, заполняющая атрибуты потока
// для вызова из "дочерних" модулей
unsigned char set_thread_attr (pthread_attr_t *attr, unsigned short priority) {

    struct sched_param schedparam;  // параметры планирования (приоритет)

    // инициализация описателя атрибутов потока управления стандартными значениями атрибутов
    if (pthread_attr_init(attr)) {
        perror("Attribute creation failed");
        return 1;
    }

   // установка атрибута "inheritsched" (стратегия планирования и связанные с ней атрибуты не наследуются - берутся из attr)
   if (pthread_attr_setinheritsched(attr, PTHREAD_EXPLICIT_SCHED)) {
       perror("Setting inheritsched failed");
       return 1;
   }

   // установка атрибута "schedpolicy" (создаваемый поток будет иметь стратегию планирования SCHED_FIFO)
   if (pthread_attr_setschedpolicy(attr, SCHED_FIFO)) {
       perror("Setting scheduling policy failed");
       return 1;
   }

   // создаваемый поток будет иметь приоритет, заданный в THREAD_ARG.priority
   schedparam.sched_priority = priority;
   if (pthread_attr_setschedparam(attr, &schedparam)) {
       perror("Setting schedparam failed");
       return 1;
   }

   return 0;
}

//*****************************************************************************
// Функция для вызова в pthread_create()
// вызывает функцию создаваемого потока из THREAD_ARG
void *start_thread (void *argv) {

    THREAD_ARG *arg = (THREAD_ARG *)argv;

    arg->isComplete = 0;

    (arg->func)(); /* вызов указанной в аргументах функции */

    arg->isComplete = 1;

    return NULL;
}

//*****************************************************************************
// Функция, создающая поток по структуре параметров потока
// для вызова из "дочерних" модулей
unsigned char create_thread (pthread_t *thread_id,
                                                pthread_attr_t *attr,
                                                THREAD_ARG *arg) {
    if (arg->isComplete) {

        if (set_thread_attr(attr, arg->priority)) {
            fprintf(stderr, "Setting %s attr failed\n", arg->name);
            return 1;
        }

        if (pthread_create(thread_id,
                                      attr,
                                      start_thread,
                                      (void *) (arg))) {
           fprintf(stderr, "Can't create %s\n", arg->name);
           return 1;
        }
        printf("%s created.\n", arg->name);

        if (pthread_attr_destroy(attr)) {
           fprintf(stderr, "Attribute destruction of %s failed", arg->name);
           return 1;
        }

    }
    else {
        fprintf(stderr, "%s isn't complete", arg->name);
        return 1;
    }

    return 0;
}




