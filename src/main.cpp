/*
   RadioLib SX127x FSK Modem Example
   This example shows how to use FSK modem in SX127x chips.
   NOTE: The sketch below is just a guide on how to use
         FSK modem, so this code should not be run directly!
         Instead, modify the other examples to use FSK
         modem and use the appropriate configuration
         methods.
   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1278 has the following connections:
#define LORA_NSS D8
#define LORA_DI00 D1
#define LORA_DI01 D2
#define LORA_RST D4
SX1278* fsk = new SX1278(new Module(LORA_NSS, LORA_DI00, LORA_RST, LORA_DI01));

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 fsk = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize SX1278 FSK modem with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  // carrier frequency:           434.0 MHz
  // bit rate:                    48.0 kbps
  // frequency deviation:         50.0 kHz
  // Rx bandwidth:                125.0 kHz
  // output power:                13 dBm
  // current limit:               100 mA
  // data shaping:                Gaussian, BT = 0.3
  // sync word:                   0x2D  0x01
  // OOK modulation:              disabled
  int state = fsk->beginFSK();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // if needed, you can switch between LoRa and FSK modes
  //
  // lora.begin()       start LoRa mode (and disable FSK)
  // lora.beginFSK()    start FSK mode (and disable LoRa)

  // the following settings can also
  // be modified at run-time
  /*state = fsk.setFrequency(433.5);
  state = fsk.setBitRate(100.0);
  state = fsk.setFrequencyDeviation(10.0);
  state = fsk.setRxBandwidth(250.0);
  state = fsk.setOutputPower(10.0);
  state = fsk.setCurrentLimit(100);
  state = fsk.setDataShaping(0.5);
  uint8_t syncWord[] = {0x01, 0x23, 0x45, 0x67,
                        0x89, 0xAB, 0xCD, 0xEF};
  state = fsk.setSyncWord(syncWord, 8);*/
  if (state != ERR_NONE) {
    Serial.print(F("Unable to set configuration, code "));
    Serial.println(state);
    while (true);
  }

}
void receive();
void transmit();

void loop() {
  // FSK modem can use the same transmit/receive methods
  // as the LoRa modem, even their interrupt-driven versions
  // NOTE: FSK modem maximum packet length is 63 bytes!


  receive();

}

void transmit() {
  // transmit FSK packet
  int state = fsk->transmit("Hello World!");
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x56,
                      0x78, 0xAB, 0xCD, 0xEF};
    int state = lora.transmit(byteArr, 8);
  */
  if (state == ERR_NONE) {
    Serial.println(F("[SX1278] Packet transmitted successfully!"));
  } else if (state == ERR_PACKET_TOO_LONG) {
    Serial.println(F("[SX1278] Packet too long!"));
  } else if (state == ERR_TX_TIMEOUT) {
    Serial.println(F("[SX1278] Timed out while transmitting!"));
  } else {
    Serial.println(F("[SX1278] Failed to transmit packet, code "));
    Serial.println(state);
  }
}


void receive() {
  // receive FSK packet
  String str;
  int state = fsk->receive(str);;
  while (state == ERR_RX_TIMEOUT) {
    state = fsk->receive(str);
    delay(1);
  }
  
  /*
    byte byteArr[8];
    int state = lora.receive(byteArr, 8);
  */
  if (state == ERR_NONE) {
    Serial.println(F("[SX1278] Received packet!"));
    Serial.print(F("[SX1278] Data:\t"));
    Serial.println(str);
  } else if (state == ERR_RX_TIMEOUT) {
    Serial.println(F("[SX1278] Timed out while waiting for packet!"));
  } else {
    Serial.println(F("[SX1278] Failed to receive packet, code "));
    Serial.println(state);
  }
}