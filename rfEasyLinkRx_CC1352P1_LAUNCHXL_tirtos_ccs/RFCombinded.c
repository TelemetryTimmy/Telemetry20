/*
 *  ======== rfEasyLinkTx.c ========
 */
/* Standard C Libraries */
#include <stdlib.h>

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
#include <mqueue.h>

/* POSIX Header files */
#include <pthread.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>

/* Board Header files */
#include "ti_drivers_config.h"

/* EasyLink API Header files */
#include "easylink/EasyLink.h"

/* Application header files */
#include <ti_radio_config.h>

#include "uartThread.h"

extern mqd_t MQ_UartOUT_RfIN;

#define RFEASYLINKTX_TASK_STACK_SIZE 1024
#define RFEASYLINKTX_TASK_PRIORITY 2

#define RFEASYLINKTX_BURST_SIZE 10
#define RFEASYLINKTXPAYLOAD_LENGTH 128

Task_Struct txTask; /* not static so you can see in ROV */
//static Task_Params txTaskParams;
//static uint8_t txTaskStack[RFEASYLINKTX_TASK_STACK_SIZE];

///* Pin driver handle */
PIN_Handle pinHandle;
extern PIN_State pinState;
//
////#define CONFIG_PIN_GLED CONFIG_PIN_3 //Green
////#define CONFIG_PIN_RLED CONFIG_PIN_0 //Red
///*
// * Application LED pin configuration table:
// *   - All LEDs board LEDs are off.
// */
//PIN_Config pinTable[] = {
//    CONFIG_PIN_GLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
//    CONFIG_PIN_RLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
//    PIN_TERMINATE
//};

//static uint16_t seqNumber;

static Semaphore_Handle rxDoneSem;

void rxDoneCb(EasyLink_RxPacket *rxPacket, EasyLink_Status status)
{
    if (status == EasyLink_Status_Success)
    {
        /* Toggle RLED to indicate RX */
        PIN_setOutputValue(pinHandle, CONFIG_PIN_RLED, !PIN_getOutputValue(CONFIG_PIN_RLED));
        //                  Display_open(Display_Type_UART, &params);
        //                  Display_printf(display, 1, 0, "Received message %d", message_count++);
        SerialWrite("Received message: ");
        //            for (uint8_t i = 0; i < rxPacket.len; i++)
        //            {
        //                radio_
        //            }
        SerialWrite((char *)rxPacket->payload);
        SerialWrite("\n");
        //                  Display_printf(display, 0, 0, "message payload %s, Packet Length = %d", rxPacket.payload,rxPacket.len);
        //                  Display_close(display);
        uint32_t i;
        for (i = 0; i < EASYLINK_MAX_DATA_LENGTH; i++)
        {
            rxPacket->payload[i] = '\0';

        }
    }
    else
    {
        /* Toggle GLED and RLED to indicate error */
        PIN_setOutputValue(pinHandle, CONFIG_PIN_GLED, !PIN_getOutputValue(CONFIG_PIN_GLED));
        PIN_setOutputValue(pinHandle, CONFIG_PIN_RLED, !PIN_getOutputValue(CONFIG_PIN_RLED));
//        message_count = 0;
    }
    Semaphore_post(rxDoneSem);
}

void RXSetup(void)
{
    /* Call driver init functions. */

    /* Open LED pins */
    //    pinHandle = PIN_open(&pinState, pinTable);
    //    Assert_isTrue(pinHandle != NULL, NULL);

    /* Clear LED pins */
    //    PIN_setOutputValue(pinHandle, CONFIG_PIN_GLED, 0);
    //    PIN_setOutputValue(pinHandle, CONFIG_PIN_RLED, 0);
}

void *rfEasyLinkTxFnx(void *arg0)
{

    mqd_t *mqdes = arg0;

    /* Create a semaphore for Async*/
    Semaphore_Params params;
    Error_Block eb;

    /* Init params */
    Semaphore_Params_init(&params);
    Error_init(&eb);

    /* Create semaphore instance */
    rxDoneSem = Semaphore_create(0, &params, &eb);
    if (rxDoneSem == NULL)
    {
        System_abort("Semaphore creation failed");
    }

    //    RXSetup();
    // Initialize the EasyLink parameters to their default values
    EasyLink_Params easyLink_params;
    EasyLink_Params_init(&easyLink_params);

    /*
     * Initialize EasyLink with the settings found in ti_easylink_config.h
     * Modify EASYLINK_PARAM_CONFIG in ti_easylink_config.h to change the default
     * PHY
     */
    if (EasyLink_init(&easyLink_params) != EasyLink_Status_Success)
    {
        System_abort("EasyLink_init failed");
    }

    /*
     * If you wish to use a frequency other than the default, use
     * the following API:
     * EasyLink_setFrequency(868000000);
     */
    EasyLink_RxPacket rxPacket = {0};
    int msgLength;
    char msg[128];
    uint32_t message_count = 0;
    while (1)
    {
        EasyLink_TxPacket txPacket = {{0}, 0, 0, {0}};

        msgLength = mq_receive(*mqdes, (char *)&msg, sizeof(msg), NULL);
        if (msgLength > 0)
        {
//            EasyLink_Status result1 = EasyLink_abort();
            int j;
            for (j = 1; j < msgLength; j++)
            {
                txPacket.payload[j - 1] = msg[j];
                if (txPacket.payload[j - 1] == '\x0d')
                    break;
            }

            int i;
            for (i = 0; i < RFEASYLINKTXPAYLOAD_LENGTH; i++)
            {
                if (txPacket.payload[i] == '\x0d')
                    break;
            }
            txPacket.len = i;

            /*
         * Address filtering is enabled by default on the Rx device with the
         * an address of 0xAA. This device must set the dstAddr accordingly.
         */
            txPacket.dstAddr[0] = 0xaa;

            EasyLink_Status result = EasyLink_transmit(&txPacket);

            if (result == EasyLink_Status_Success)
            {
                /* Toggle GLED to indicate TX */
                SerialWrite("RX Success\n");
                //            PIN_setOutputValue(pinHandle, CONFIG_PIN_GLED,!PIN_getOutputValue(CONFIG_PIN_GLED));
            }
            else
            {
                SerialWrite("Rx Fail\n");
                /* Toggle GLED and RLED to indicate error */
                //            PIN_setOutputValue(pinHandle, CONFIG_PIN_GLED,!PIN_getOutputValue(CONFIG_PIN_GLED));
                //            PIN_setOutputValue(pinHandle, CONFIG_PIN_RLED,!PIN_getOutputValue(CONFIG_PIN_RLED));
            }
        }
        else
        {
            EasyLink_receiveAsync(rxDoneCb, 0);

            /* Wait 300ms for Rx */
            if (Semaphore_pend(rxDoneSem, (300000 / Clock_tickPeriod)) == FALSE)
            {
                /* RX timed out abort */
                if (EasyLink_abort() == EasyLink_Status_Success)
                {
                    /* Wait for the abort */
                    Semaphore_pend(rxDoneSem, BIOS_WAIT_FOREVER);
                }
            }

            rxPacket.absTime = 0;

            //                          Task_sleep(5000);
        }
                Task_sleep(500);
    }
}

//void txTask_init(PIN_Handle inPinHandle) {
//    pinHandle = inPinHandle;
//
//    Task_Params_init(&txTaskParams);
//    txTaskParams.stackSize = RFEASYLINKTX_TASK_STACK_SIZE;
//    txTaskParams.priority = RFEASYLINKTX_TASK_PRIORITY;
//    txTaskParams.stack = &txTaskStack;
//    txTaskParams.arg0 = (UInt)1000000;
//    txTaskParams.arg1 = (void *)&MQ_UartOUT_RfIN;
//
//    Task_construct(&txTask, rfEasyLinkTxFnx, &txTaskParams, NULL);
//}

/*
 *  ======== main ========
 */
