/*
 *  ======== main_tirtos.c ========
 */
#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>
#include <mqueue.h> //Message q for inter thread communication

/* Driver configuration */
#include <ti/drivers/Board.h>

/* User libraries*/
#include "uartThread.h"
#include "rfEasyLinkTx.h"

//extern void *mainThread(void *arg0);
//extern void *uartThread(void *arg0);

/* Stack size in bytes */
#define THREADSTACKSIZE    1024

// Message Queue's
#define MSG_SIZE (sizeof(uint8_t)*UARTBUFFERSIZE)
#define MSG_NUM  4

mqd_t MQ_UartOUT_RfIN;// message q to send to RF thread from uart thread


/*
 *  ======== main ========
 */
int main(void)
{
    pthread_t           UART,rfThread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    struct mq_attr      mqAttrs_UartOUT_RfIN;

    /* Call driver init functions */
    Board_init();
    /* Message Queue init */
    mqAttrs_UartOUT_RfIN.mq_maxmsg = MSG_NUM;
    mqAttrs_UartOUT_RfIN.mq_msgsize = MSG_SIZE;
    mqAttrs_UartOUT_RfIN.mq_flags = 0;
    MQ_UartOUT_RfIN = mq_open ("alarm", O_RDWR | O_CREAT,
                    0664, &mqAttrs_UartOUT_RfIN);
    if (MQ_UartOUT_RfIN == (mqd_t)-1) {
      /* mq_open() failed */
      while (1);
    }
    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }

    retc = pthread_create(&rfThread, &attrs, rfEasyLinkTxFnx, (void *)&MQ_UartOUT_RfIN);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }
    /* Set UART thread Priority and create thread */
    priParam.sched_priority = 2;
    retc = pthread_create(&UART, &attrs, uartThread, (void *)&MQ_UartOUT_RfIN);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }


    BIOS_start();

    return (0);
}
