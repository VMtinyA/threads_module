#include "abstract_thread.h"

//*****************************************************************************
// Функция, заполняющая атрибуты потока
// для вызова из "дочерних" модулей
unsigned char set_thread_attr (THREAD_ARG *param) {

    struct sched_param schedparam;  // параметры планирования (приоритет)

    // инициализация описателя атрибутов потока управления стандартными значениями атрибутов
    if (pthread_attr_init(&(param->attr))) {
        perror("Attribute creation failed");
        return 1;
    }

   // установка атрибута "inheritsched" (стратегия планирования и связанные с ней атрибуты не наследуются - берутся из attr)
   if (pthread_attr_setinheritsched(&(param->attr), PTHREAD_EXPLICIT_SCHED)) {
       perror("Setting inheritsched failed");
       return 1;
   }

   // установка атрибута "schedpolicy" (создаваемый поток будет иметь стратегию планирования SCHED_FIFO)
   if (pthread_attr_setschedpolicy(&(param->attr), SCHED_FIFO)) {
       perror("Setting scheduling policy failed");
       return 1;
   }

   // создаваемый поток будет иметь приоритет, заданный в THREAD_ARG.priority
   schedparam.sched_priority = param->priority;
   if (pthread_attr_setschedparam(&(param->attr), &schedparam)) {
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
