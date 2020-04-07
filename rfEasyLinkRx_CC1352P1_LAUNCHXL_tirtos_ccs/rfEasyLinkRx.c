/*
 *  ======== rfEasyLinkRx.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>

/* POSIX Header files */
#include <pthread.h>
#include <mqueue.h> //Message q for inter thread communication

/* Stack size in bytes */
#define THREADSTACKSIZE    1024

/* Ti drivers*/
#include <ti/display/Display.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>

/* Board Header files */
#include "ti_drivers_config.h"

/* EasyLink API Header files */
#include "easylink/EasyLink.h"

/* User Libs */
#include "uartThread.h"
/***** Defines *****/



#define RFEASYLINKRX_TASK_STACK_SIZE    1024
#define RFEASYLINKRX_TASK_PRIORITY      2




/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */


/***** Variable declarations *****/
//static Task_Params rxTaskParams;
//Task_Struct rxTask;    /* not static so you can see in ROV */
//static uint8_t rxTaskStack[RFEASYLINKRX_TASK_STACK_SIZE];

/* The RX Output struct contains statistics about the RX operation of the radio */
PIN_Handle pinHandle;



 bool RXMode = false;
 void *rfEasyLinkRxFnx(void *arg0)
{
//    Display_Handle    display;
//    Display_Params    params;
//
//    Display_Params_init(&params);
//    display = Display_open(Display_Type_UART, &params);
//    if (display == NULL) {
//        // Display_open() failed
//        while(1);
//    }
//
//    Display_printf(display, 1, 0, "Display Driver Booted");
//    Display_close(display);

    // Initialize the EasyLink parameters to their default values
    EasyLink_Params easyLink_params;
    EasyLink_Params_init(&easyLink_params);
    // Create an RX packet
     EasyLink_RxPacket rxPacket = {0};

    /*
     * Initialize EasyLink with the settings found in ti_easylink_config.h
     * Modify EASYLINK_PARAM_CONFIG in ti_easylink_config.h to change the default
     * PHY
     */
    if(EasyLink_init(&easyLink_params) != EasyLink_Status_Success)
    {
        System_abort("EasyLink_init failed");
    }

    /*
     * If you wish to use a frequency other than the default, use
     * the following API:
     * EasyLink_setFrequency(868000000);
     */
    uint32_t message_count = 0;
    SerialWrite("RX Online\n");
    while(1) {
        if (RXMode)
        {
        rxPacket.absTime = 0;
        int i;
        for (i = 0; i < EASYLINK_MAX_DATA_LENGTH; i++)
        {
            rxPacket.payload[i] = '\00';
        }
              EasyLink_Status result = EasyLink_receive(&rxPacket);

              if (result == EasyLink_Status_Success)
              {
                  /* Toggle RLED to indicate RX */
                  PIN_setOutputValue(pinHandle, CONFIG_PIN_RLED,!PIN_getOutputValue(CONFIG_PIN_RLED));
//                  Display_open(Display_Type_UART, &params);
//                  Display_printf(display, 1, 0, "Received message %d", message_count++);
                  SerialWrite("Received message: ");
      //            for (uint8_t i = 0; i < rxPacket.len; i++)
      //            {
      //                radio_
      //            }
                  SerialWrite((char *)rxPacket.payload);
                  SerialWrite("\n");
//                  Display_printf(display, 0, 0, "message payload %s, Packet Length = %d", rxPacket.payload,rxPacket.len);
//                  Display_close(display);



              }
              else
              {
                  /* Toggle GLED and RLED to indicate error */
                  PIN_setOutputValue(pinHandle, CONFIG_PIN_GLED,!PIN_getOutputValue(CONFIG_PIN_GLED));
                  PIN_setOutputValue(pinHandle, CONFIG_PIN_RLED,!PIN_getOutputValue(CONFIG_PIN_RLED));
                  message_count = 0;
              }
        }
        else
             Task_sleep(5000);

    }
}

