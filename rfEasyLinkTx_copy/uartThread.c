/*
 *  ======== uartThread.c ========
 */

/*
 *  ======== uartecho.c ========
 */
#include <stdint.h>
#include <stddef.h>

#include "uartThread.h"
/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <mqueue.h>

/* Driver configuration */
#include "ti_drivers_config.h"

extern mqd_t MQ_UartOUT_RfIN;

/*
 *  ======== mainThread ========
 */
UART_Handle uart;
void *uartThread(void *arg0)
{
    char        input;
    const char  echoPrompt[] = "Echoing characters:\r\n";

    UART_Params uartParams;
    mqd_t      *mqdes = arg0;

    UART_init();


    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    UART_write(uart, echoPrompt, sizeof(echoPrompt));

    /* Loop forever echoing */
    uint8_t buffer[UARTBUFFERSIZE];
    int i = 0;
    while (1) {

        UART_read(uart, &input, 1);

        buffer[i] = input;
        UART_write(uart, &input, 1);

        if (buffer[i] == '\x0d')
        {
//            UART_write(uart, &buffer, i);
            i = 0;
            buffer[i] = '\x0d';
            UART_write(uart, "\x0d", 1);
            mq_send(*mqdes , (char *)buffer, sizeof(buffer), 0);
            SerialWrite("message sent to radio\n\0");
        }
        i++;
    }
}


extern mqd_t MQ_Uart_Sent;

void *uartsendThread(void *arg0)
{
    char msg[128];
    int msglen;
    while (1)
    {
        mq_receive(MQ_Uart_Sent, (char *)msg, sizeof(msg), NULL);
        for (msglen = 0; msglen < 128; msglen++)
        {
            if (msg[msglen] == '\0')
                break;
        }

        UART_write(uart, msg, msglen);
    }
}

void SerialWrite(char* message)
{
    mq_send(MQ_Uart_Sent , (char *)message, sizeof(message), 0);
}
