#include <Arduino.h>

#include <SPI.h>
// #include <LoRa.h>
#include "mcp_can.h"
#include <nRF24L01.h>
#include <RF24.h>
#include "stdio.h"

const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  // Serial.println("LoRa Sender");
  // LoRa.setPins(8, 9, 2);
  // if (!LoRa.begin(915E6))
  // {
  //   Serial.println("Starting LoRa failed!");
  //   while (1)
  //     ;
  // }
  while (CAN_OK != CAN.begin(CAN_1000KBPS)) // init can bus : baudrate = 500k
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(1000);
  }
  Serial.println("CAN BUS Shield init ok!");
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

}

void loop()
{
  unsigned char len = 0;
  unsigned char buf[8];
  //delay(1000);
  if (CAN_MSGAVAIL == CAN.checkReceive()) // check if data coming
  {
    CAN.readMsgBuf(&len, buf); // read data,  len: data length, buf: data buf

    unsigned int canId = CAN.getCanId();
    if (canId == 0x190 && buf[0] == 0x30)
    {
      Serial.println("-----------------------------");
      Serial.print("Get data from ID: ");
      Serial.print(canId, HEX);
      Serial.print(" ");
      for (int i = 0; i < len; i++) // print the data
      {

        Serial.print(buf[i], HEX);
        Serial.print("\t");
      }
      if( !radio.write(&buf, sizeof(buf)))
      {
        Serial.println("Radio error");
        while(1){}
      }
      else
      {
      Serial.println("data sent");
        /* code */
      }
      
      delay(500);
      // LoRa.beginPacket();
      // for (int i = 0; i < len; i++) // print the data
      // {

      //   LoRa.print(buf[i], HEX);
      //   LoRa.print("\t");
      // }
      // LoRa.endPacket();
      //Serial.println();
    }
  }

  //delay(5000);
}