#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5);
uint8_t txAddress[6] = "1Node";

bool radioNumber;
bool role = false;  // true = TX node, false = RX node
uint8_t const payloadSize = 5;
uint8_t bufferTx[payloadSize + 1] = "12345";
uint16_t failuresTx = 0;

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

  radio.openWritingPipe(txAddress);
  radio.stopListening();
}

void loop() {
  radio.flush_tx();
  if (!radio.writeFast(&bufferTx, payloadSize)) {
    Serial.println(F("Failed to transmit data to receiver"));
    failuresTx++;
    radio.reUseTX();
  } else {
    Serial.println(F("Successfully transmit data to receiver"));
  }

  if (failuresTx >= 100) {
    Serial.println(F("Too many failures detected"));
  }

  delay(5000);
}
