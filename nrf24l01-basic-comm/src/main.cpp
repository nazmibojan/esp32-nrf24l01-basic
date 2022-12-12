#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define TX_MODE

RF24 radio(4, 5);

uint8_t const payloadSize = 5;
const uint64_t address = 0xF0F0F0F0E1LL;

#ifdef TX_MODE
uint8_t bufferTx[payloadSize + 1] = "12345";
uint16_t failuresTx = 0;
#else
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
  radio.setPayloadSize(payloadSize);

#ifdef TX_MODE
  radio.openWritingPipe(address);
  radio.stopListening();
#else
  radio.openReadingPipe(0, address);
  radio.startListening();
  radio.printDetails();
#endif
}

void loop() {
#ifdef TX_MODE
  radio.write(&bufferTx, sizeof(bufferTx));
  delay(5000);
#else
  if (radio.available()) {
    radio.read(bufferRx, payloadSize);
    snprintf(burfferRxPrint, payloadSize + 1, "%s", bufferRx);

    Serial.print(F("Received Data: "));
    Serial.print(burfferRxPrint);
    Serial.print(F(" - "));
    Serial.println(counterRx++);
  }
#endif
}
