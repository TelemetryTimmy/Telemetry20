/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
//    /* 1 second delay */
//    uint32_t time = 1;
//
//    /* Call driver init functions */
//    GPIO_init();
//    // I2C_init();
//    // SPI_init();
//    // UART_init();
//    // Watchdog_init();
//
//    /* Configure the LED pin */
//    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//
//    /* Turn on user LED */
//    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
//
//    while (1) {
//        sleep(time);
//        GPIO_toggle(CONFIG_GPIO_LED_0);
//    }
}
