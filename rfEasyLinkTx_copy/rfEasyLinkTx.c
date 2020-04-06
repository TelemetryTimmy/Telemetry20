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


extern mqd_t MQ_UartOUT_RfIN;

#define RFEASYLINKTX_TASK_STACK_SIZE    1024
#define RFEASYLINKTX_TASK_PRIORITY      2

#define RFEASYLINKTX_BURST_SIZE         10
#define RFEASYLINKTXPAYLOAD_LENGTH      128

Task_Struct txTask;    /* not static so you can see in ROV */
//static Task_Params txTaskParams;
//static uint8_t txTaskStack[RFEASYLINKTX_TASK_STACK_SIZE];

/* Pin driver handle */
static PIN_Handle pinHandle;
static PIN_State pinState;

//#define CONFIG_PIN_GLED CONFIG_PIN_3 //Green
//#define CONFIG_PIN_RLED CONFIG_PIN_0 //Red
/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config pinTable[] = {
    CONFIG_PIN_GLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    CONFIG_PIN_RLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

//static uint16_t seqNumber;

void RXSetup(void)
{
    /* Call driver init functions. */


    /* Open LED pins */
    pinHandle = PIN_open(&pinState, pinTable);
    Assert_isTrue(pinHandle != NULL, NULL);

    /* Clear LED pins */
    PIN_setOutputValue(pinHandle, CONFIG_PIN_GLED, 0);
    PIN_setOutputValue(pinHandle, CONFIG_PIN_RLED, 0);
}


void *rfEasyLinkTxFnx(void *arg0)
{
    uint8_t txBurstSize = 0;
    uint32_t absTime;
    mqd_t      *mqdes = arg0;

    RXSetup();
    // Initialize the EasyLink parameters to their default values
    EasyLink_Params easyLink_params;
    EasyLink_Params_init(&easyLink_params);

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
    int msgLength;
    char msg[128];
    while(1) {
        EasyLink_TxPacket txPacket =  { {0}, 0, 0, {0} };

        msgLength = mq_receive(*mqdes, (char *)&msg, sizeof(msg), NULL);
        int j;
        for ( j=1; j < msgLength; j++)
        {
            txPacket.payload[j-1] = msg[j];
            if (txPacket.payload[j-1] == '\x0d')
                break;
        }
//        txPacket.payload[0] = (uint8_t)'F';
//        txPacket.payload[1] = (uint8_t)'u';
//        txPacket.payload[2] = (uint8_t)'c';
//        txPacket.payload[3] = (uint8_t)'k';
//        txPacket.payload[4] = (uint8_t)'\0';
////        uint8_t i;
////        for (i = 2; i < RFEASYLINKTXPAYLOAD_LENGTH; i++)
////        {
////            txPacket.payload[i] = rand();
////        }
        int i;
        for (i=0; i < RFEASYLINKTXPAYLOAD_LENGTH; i++){
            if ( txPacket.payload[i] == '\x0d')
                break;
        }
        txPacket.len = i;

        /*
         * Address filtering is enabled by default on the Rx device with the
         * an address of 0xAA. This device must set the dstAddr accordingly.
         */
        txPacket.dstAddr[0] = 0xaa;

//        /* Add a Tx delay for > 500ms, so that the abort kicks in and brakes the burst */
//        if(EasyLink_getAbsTime(&absTime) != EasyLink_Status_Success)
//        {
//            // Problem getting absolute time
//        }
//        if(txBurstSize++ >= RFEASYLINKTX_BURST_SIZE)
//        {
//          /* Set Tx absolute time to current time + 1s */
//          txPacket.absTime = absTime + EasyLink_ms_To_RadioTime(1000);
//          txBurstSize = 0;
//        }
//        /* Else set the next packet in burst to Tx in 100ms */
//        else
//        {
//          /* Set Tx absolute time to current time + 100ms */
//          txPacket.absTime = absTime + EasyLink_ms_To_RadioTime(100);
//        }


        EasyLink_Status result = EasyLink_transmit(&txPacket);

        if (result == EasyLink_Status_Success)
        {
            /* Toggle GLED to indicate TX */
            PIN_setOutputValue(pinHandle, CONFIG_PIN_GLED,!PIN_getOutputValue(CONFIG_PIN_GLED));
        }
        else
        {
            /* Toggle GLED and RLED to indicate error */
            PIN_setOutputValue(pinHandle, CONFIG_PIN_GLED,!PIN_getOutputValue(CONFIG_PIN_GLED));
            PIN_setOutputValue(pinHandle, CONFIG_PIN_RLED,!PIN_getOutputValue(CONFIG_PIN_RLED));
        }

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
