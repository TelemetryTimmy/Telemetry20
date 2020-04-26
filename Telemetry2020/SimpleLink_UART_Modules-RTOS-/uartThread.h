/*
 * uartThread.h
 *
 *  Created on: 3 Apr. 2020
 *      Author: Baker
 */

#ifndef UARTTHREAD_H_
#define UARTTHREAD_H_

#define UARTBUFFERSIZE 128
void *uartThread(void *arg0);
void *uartsendThread(void *arg0);
void SerialWrite(char message[128]);

#endif /* UARTTHREAD_H_ */
