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
  while (!Serial);

  // Serial.println("LoRa Sender");
  // LoRa.setPins(8, 9, 2);
  // if (!LoRa.begin(915E6))
  // {
  //   Serial.println("Starting LoRa failed!");
  //   while (1)
  //     ;
  // }


  if (!radio.begin())
  {
    Serial.println("Radio Error");
    while(1){}
  }
  radio.setChannel(120);
  radio.openReadingPipe(0, address);
  //radio.openReadingPipe(1, Controler_2_Address);

  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("startup done");
}

void loop()
{
  unsigned char len = 0;
  unsigned char buf[8];
  if (radio.available())
  {

    radio.read(&buf, sizeof(buf));
    Serial.println("-----------------------------");
    Serial.print(" ");
    for (int i = 0; i < 7; i++) // print the data
    {

      Serial.print(buf[i], HEX);
      Serial.print("\t");
    }
  }
  // else
  // {
  //   Serial.println("No Data");
  //   delay(1000);
  // }
  

  //delay(5000);
}