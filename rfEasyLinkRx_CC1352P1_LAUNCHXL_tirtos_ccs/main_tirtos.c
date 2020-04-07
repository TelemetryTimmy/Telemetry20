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
#include "ti_drivers_config.h"
#include <ti/drivers/Board.h>
#include <ti/drivers/GPIO.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>

/* EasyLink API Header files */
#include "easylink/EasyLink.h"

/* User libraries*/
#include "uartThread.h"
#include "rfEasyLinkRx.h"
//#include "rfEasyLinkTx.h"
#include "RFCombinded.h"


/* Stack size in bytes */
#define THREADSTACKSIZE    1024


// Message Queue's
#define MSG_SIZE (sizeof(uint8_t)*UARTBUFFERSIZE)
#define MSG_NUM  100

mqd_t MQ_UartOUT_RfIN;// message q to send to RF thread from uart thread
mqd_t MQ_Uart_Sent;


/* Pin driver handle */
 PIN_Handle ledPinHandle;
 PIN_State ledPinState;

PIN_Config pinTable[] = {
    CONFIG_PIN_GLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    CONFIG_PIN_RLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};
/*
 *  ======== main ========
 */
int main(void)
{
    pthread_t           rfThread,RFSend,UART,UARTSend;
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
    MQ_UartOUT_RfIN = mq_open ("uartin", O_RDWR | O_CREAT | O_NONBLOCK,
                    0664, &mqAttrs_UartOUT_RfIN);
    if (MQ_UartOUT_RfIN == (mqd_t)-1) {
      /* mq_open() failed */
      while (1);
    }
    MQ_Uart_Sent = mq_open ("uartout", O_RDWR | O_CREAT,
                    0664, &mqAttrs_UartOUT_RfIN);
    if (MQ_UartOUT_RfIN == (mqd_t)-1) {
      /* mq_open() failed */
      while (1);
    }

    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, pinTable);
    //Assert_isTrue(ledPinHandle != NULL, NULL);

    /* Clear LED pins */
    PIN_setOutputValue(ledPinHandle, CONFIG_PIN_GLED, 0);
    PIN_setOutputValue(ledPinHandle, CONFIG_PIN_RLED, 0);

//    rxTask_init(ledPinHandle);
    /* Initialize the attributes structure with default values */
      pthread_attr_init(&attrs);

      /* Set priority, detach state, and stack size attributes */
      priParam.sched_priority = 4;
      retc = pthread_attr_setschedparam(&attrs, &priParam);
      retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
      retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
      if (retc != 0) {
          /* failed to set attributes */
          while (1) {}
      }

//      retc = pthread_create(&rfThread, &attrs, rfEasyLinkRxFnx, (void *)&MQ_UartOUT_RfIN);
      if (retc != 0) {
          /* pthread_create() failed */
          while (1) {}
      }

      /* Set UART thread Priority and create thread */
      priParam.sched_priority = 1;
      retc = pthread_create(&UART, &attrs, uartThread, (void *)&MQ_UartOUT_RfIN);
      if (retc != 0) {
          /* pthread_create() failed */
          while (1) {}
      }

      priParam.sched_priority = 2;
      retc = pthread_create(&UARTSend, &attrs, uartsendThread, NULL);
      if (retc != 0) {
          /* pthread_create() failed */
          while (1) {}
      }

      priParam.sched_priority = 3;
      retc = pthread_create(&RFSend, &attrs, rfEasyLinkTxFnx, (void *)&MQ_UartOUT_RfIN);
      if (retc != 0) {
          /* pthread_create() failed */
          while (1) {}
      }

    /* Start BIOS */
    BIOS_start();

    return (0);
}
