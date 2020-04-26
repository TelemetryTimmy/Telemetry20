/*
 * TCAN4x5x_SPI.c
 * Description: This file is responsible for abstracting the lower-level microcontroller SPI read and write functions
 *
 *
 *
 * Copyright (c) 2019 Texas Instruments Incorporated.  All rights reserved.
 * Software License Agreement
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>

#include "tcan4x5x/TCAN4x5x_SPI.h"
/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti_drivers_config.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include "uartThread.h"

/* RTOS Libs */
#include <semaphore.h>

#define SPI_HANDLE masterSpi
#define SPI_SEMAPHORE SPISem
#define SPI_CS_PIN CONFIG_nCS

#define LOW 0
#define HIGH 1
#define SPI_MSG_LENGTH  30

SPI_Handle      SPI_HANDLE;
uint8_t spi_Status;
sem_t SPI_SEMAPHORE;

// Removed until callback mode is implimented
//SPI_CallbackFxn
//spi_CallBack(void *arg0)
//{
//    bool *spi_Status = arg0;
//    if (spi_Status)
//        sem_post(&SPI_SEMAPHORE);
//    else
//        while(1){}
//

/*
 * @brief Single word write
 *
 * @param address A 16-bit address of the destination register
 * @param data A 32-bit word of data to write to the destination register
 */
void
AHB_WRITE_32(uint16_t address, uint32_t data)
{
    AHB_WRITE_BURST_START(address, 1);
//    Task_sleep(500);
    AHB_WRITE_BURST_WRITE(data);
    AHB_WRITE_BURST_END();
}


/*
 * @brief Single word read
 *
 * @param address A 16-bit address of the source register
 *
 * @return Returns 32-bit word of data from source register
 */
uint32_t
AHB_READ_32(uint16_t address)
{
    uint32_t returnData;

    AHB_READ_BURST_START(address, 1);
//    Task_sleep(500);
    returnData = AHB_READ_BURST_READ();
//    Task_sleep(500);
    AHB_READ_BURST_END();

    return returnData;
}


/*
 * @brief Burst write start
 *
 * The SPI transaction contains 3 parts: the header (start), the payload, and the end of data (end)
 * This function is the start, where the register address and number of words are transmitted
 *
 * @param address A 16-bit address of the destination register
 * @param words The number of 4-byte words that will be transferred. 0 = 256 words
 */
void
AHB_WRITE_BURST_START(uint16_t address, uint8_t nbofBytes)
{
    int spi_Status;
    SPI_Transaction transaction;
    unsigned char rxBuffer[SPI_MSG_LENGTH] = {0};
    unsigned char txBuffer[SPI_MSG_LENGTH] = {0};// init to zero so there is no garbage.


    transaction.count = 4;

    txBuffer[0] = AHB_WRITE_OPCODE; //COMMAND BYTE
    txBuffer[1] = address>>8; //16BIT ADDRESS, Where this data is going RX or TX fifo?
    txBuffer[2] = (uint8_t)(address & 0x00FF);

    // transaction.arg = &spi_Status;
    txBuffer[3] = nbofBytes; // How many bytes are going there
    transaction.txBuf =  (void*)txBuffer;
    transaction.rxBuf = (void*)rxBuffer;


    //set the CS low to start the transaction
    GPIO_write(SPI_CS_PIN, LOW);

    //Start the transaction
    spi_Status = SPI_transfer(SPI_HANDLE, &transaction);

    if (!spi_Status)
        while (1){} // Fault occurred stay here forever.
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, AHB_WRITE_OPCODE); // SEND COMMAND BYTE
//
//    // Send the 16-bit address
//    WAIT_FOR_TRANSMIT;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, HWREG8(&address + 1)); //SEND ADDRESS BYTE 1 (HIGH BYTE)
//    WAIT_FOR_TRANSMIT;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, HWREG8(&address)); //SEND ADDRESS BYTE 2 (LOW BYTE)
//
//
//    WAIT_FOR_TRANSMIT;
//    // Send the number of words to read
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, words);   //SEND THE NUMBER OF BYTES TO BE SENT

}


/*
 * @brief Burst write
 *
 * The SPI transaction contains 3 parts: the header (start), the payload, and the end of data (end)
 * This function writes a single word at a time
 *
 * @param data A 32-bit word of data to write to the destination register
 */
void
AHB_WRITE_BURST_WRITE(uint32_t data)
{
    bool spi_Status;
    unsigned char rxBuffer[SPI_MSG_LENGTH] = {0};
    unsigned char txBuffer[SPI_MSG_LENGTH] = {0};// init to zero so there is no garbage.

    txBuffer[3] = ((data & 0x000000ff));
    txBuffer[2] = ((data & 0x0000ff00)>>8);
    txBuffer[1] = ((data & 0x00ff0000)>>16);
    txBuffer[0] = ((data & 0xff000000)>>24);
       SPI_Transaction transaction = {0}; // init to zero so there is no garbage.
       transaction.count = 4;
       transaction.txBuf = (void *) txBuffer; //Data to be shifted out
       transaction.rxBuf = (void *) rxBuffer;
       transaction.arg = &spi_Status;



       //Start the transaction
       spi_Status = SPI_transfer(SPI_HANDLE, &transaction);
       if (transaction.status != SPI_TRANSFER_COMPLETED)
           while (1){} // Fault occurred stay here forever.

//    WAIT_FOR_TRANSMIT;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, HWREG8(&data + 3));
//    WAIT_FOR_TRANSMIT;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, HWREG8(&data + 2));
//    WAIT_FOR_TRANSMIT;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, HWREG8(&data + 1));
//    WAIT_FOR_TRANSMIT;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, HWREG8(&data));
}


/*
 * @brief Burst write end
 *
 * The SPI transaction contains 3 parts: the header (start), the payload, and the end of data (end)
 * This function ends the burst transaction by pulling nCS high
 */
void
AHB_WRITE_BURST_END(void)
{
//    sem_wait(&SPI_SEMAPHORE);
    GPIO_write(SPI_CS_PIN, HIGH);
}


/*
 * @brief Burst read start
 *
 * The SPI transaction contains 3 parts: the header (start), the payload, and the end of data (end)
 * This function is the start, where the register address and number of words are transmitted
 *
 * @param address A 16-bit start address to begin the burst read
 * @param words The number of 4-byte words that will be transferred. 0 = 256 words
 */
void
AHB_READ_BURST_START(uint16_t address, uint8_t nbofBytes)
{
    bool spi_Status;
    SPI_Transaction transaction = {0}; // init to zero so there is no garbage.
    unsigned char txBuffer[SPI_MSG_LENGTH] = {0};// init to zero so there is no garbage.
    unsigned char rxBuffer[SPI_MSG_LENGTH] = {0};// init to zero so there is no garbage.
    transaction.count = 4;
    txBuffer[0] = AHB_READ_OPCODE; //COMMAND BYTE
    txBuffer[1] = address>>8; //16BIT ADDRESS, Where this data is going RX or TX fifo?
    txBuffer[2] = (uint8_t)(address & 0x00FF);
    txBuffer[3] = nbofBytes; // How many bytes are going there

    transaction.txBuf = (void *)txBuffer;
    transaction.rxBuf = (void *)rxBuffer;
    transaction.arg = &spi_Status;

    //set the CS low to start the transaction
    GPIO_write(SPI_CS_PIN, LOW);

    //Start the transaction
    spi_Status = SPI_transfer(SPI_HANDLE, &transaction);
//    Task_sleep(500);

//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, AHB_READ_OPCODE);
//
//    // Send the 16-bit address
//    WAIT_FOR_TRANSMIT;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, HWREG8(&address + 1));
//    WAIT_FOR_TRANSMIT;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, HWREG8(&address));
//
//    // Send the number of words to read
//    WAIT_FOR_TRANSMIT;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, words);

}


/*
 * @brief Burst read start
 *
 * The SPI transaction contains 3 parts: the header (start), the payload, and the end of data (end)
 * This function where each word of data is read from the TCAN4x5x
 *
 * @return A 32-bit single data word that is read at a time
 */
uint32_t
AHB_READ_BURST_READ(void)
{
    bool spi_Status;
    SPI_Transaction transaction = {0}; // init to zero so there is no garbage.

    unsigned char txBuffer[SPI_MSG_LENGTH] = {0};// init to zero so there is no garbage.
    unsigned char rxBuffer[SPI_MSG_LENGTH] = {0};// init to zero so there is no garbage.
       transaction.count = 4;
       transaction.txBuf = (void *)txBuffer;
       transaction.rxBuf = (void *)rxBuffer;
       transaction.arg = &spi_Status;
       transaction.status;



       //Start the transaction
       spi_Status = SPI_transfer(SPI_HANDLE, &transaction);
//    uint8_t readData;
//    uint8_t readData1;
//    uint8_t readData2;
//    uint8_t readData3;
//    uint32_t returnData;
//
//    WAIT_FOR_IDLE;
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, 0x00); // pause after this
//    WAIT_FOR_IDLE;
//
//    readData = HWREG8(SPI_HW_ADDR + OFS_UCBxRXBUF);
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, 0x00);
//
//
//    WAIT_FOR_IDLE;
//    readData1 = HWREG8(SPI_HW_ADDR + OFS_UCBxRXBUF);
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, 0x00);
//
//    WAIT_FOR_IDLE;
//    readData2 = HWREG8(SPI_HW_ADDR + OFS_UCBxRXBUF);
//    EUSCI_B_SPI_transmitData(SPI_HW_ADDR, 0x00);
//
//    WAIT_FOR_IDLE;
//    readData3 = HWREG8(SPI_HW_ADDR + OFS_UCBxRXBUF);
//
//
   uint32_t returnData = (((uint32_t)rxBuffer[0]) << 24 |  ((uint32_t)rxBuffer[1]) << 16 |  ((uint32_t)rxBuffer[2]) << 8 |  (uint32_t)rxBuffer[3]);
    return returnData;
}


/*
 * @brief Burst write end
 *
 * The SPI transaction contains 3 parts: the header (start), the payload, and the end of data (end)
 * This function ends the burst transaction by pulling nCS high
 */
void
AHB_READ_BURST_END(void)
{
//    sem_wait(&SPI_SEMAPHORE);removed while spi is functioning in blocking mode
    GPIO_write(SPI_CS_PIN, HIGH);
}

/*
 * @brief Inits the spi module and opens an spi port
 *
 */
void
SPI_Setup(void)
{
    SPI_init();
    SPI_Params      spiParams;
    int32_t         status;

    /*
     * Create synchronization semaphore; the master will wait on this semaphore
     * until the slave is ready.
     */
    status = sem_init(&SPI_SEMAPHORE, 0, 0);
    if (status != 0) {
        SerialWrite("Error creating masterSem\n");

        while(1);
    }

    /* Open SPI as master (default) */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA0;
    spiParams.bitRate = 2000000;
//    spiParams.transferMode = ??????
//    spiParams.transferCallbackFxn = spi_CallBack;  //Function not passing in correctly, use blocking mode for now.
//    spiParams.transferMode = SPI_MODE_CALLBACK;
    SPI_HANDLE = SPI_open(CONFIG_SPI_MASTER, &spiParams);
    if (masterSpi == NULL) {
        SerialWrite("Error initializing master SPI\n");
        while (1){}
    }
    else {
       SerialWrite("Master SPI initialized\n");
    }


}
