#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define TX_MODE
#define ENABLE_ACK

RF24 radio(4, 5);

uint8_t const payloadSize = 5;
#ifdef ENABLE_ACK
const uint64_t addrFirst = 0xF0F0F0F0E1LL;
const uint64_t addrSecond = 0xF0F0F0F0E3LL;
#else
const uint64_t address = 0xF0F0F0F0E1LL;
#endif

#ifdef TX_MODE
uint8_t bufferTx[payloadSize + 1] = "12345";
#ifdef ENABLE_ACK
uint8_t bufferRx[payloadSize + 1] = {0};
char burfferRxPrint[payloadSize + 1] = "";
uint16_t counterRx;
#endif
#else
#ifdef ENABLE_ACK
uint8_t bufferAck[payloadSize + 1] = "67890";
#endif
uint8_t bufferRx[payloadSize + 1] = {0};
char burfferRxPrint[payloadSize + 1] = "";
uint16_t counterRx;
#endif

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  } else {
    Serial.println(F("radio hardware is successfully init"));
  }

  radio.setPALevel(RF24_PA_LOW);
#ifdef ENABLE_ACK
  radio.enableDynamicPayloads();
  radio.enableAckPayload();

#ifdef TX_MODE
  radio.openWritingPipe(addrFirst);
  radio.openReadingPipe(1, addrSecond);
  radio.stopListening();
#else
  radio.openReadingPipe(0, addrFirst);
  radio.openWritingPipe(addrSecond);
  // radio.writeAckPayload(1, &bufferAck, sizeof(bufferAck));
  radio.startListening();
#endif
#else
  radio.setPayloadSize(payloadSize);

#ifdef TX_MODE
  radio.openWritingPipe(address);
  radio.stopListening();
#else
  radio.openReadingPipe(0, address);
  radio.startListening();
  radio.printDetails();
#endif
#endif
}

void loop() {
#ifdef TX_MODE
  radio.write(&bufferTx, sizeof(bufferTx));
  delay(5000);

#ifdef ENABLE_ACK
  if (radio.available()) {
    radio.read(bufferRx, payloadSize);
    snprintf(burfferRxPrint, payloadSize + 1, "%s", bufferRx);

    Serial.print(F("Received Data: "));
    Serial.print(burfferRxPrint);
    Serial.print(F(" - "));
    Serial.println(counterRx++);
  }
  #endif
#else
  if (radio.available()) {
    radio.read(bufferRx, payloadSize);
    snprintf(burfferRxPrint, payloadSize + 1, "%s", bufferRx);

    Serial.print(F("Received Data: "));
    Serial.print(burfferRxPrint);
    Serial.print(F(" - "));
    Serial.println(counterRx++);

#ifdef ENABLE_ACK
    radio.writeAckPayload(1, &bufferAck, sizeof(bufferAck));
#endif
  }
#endif
}
