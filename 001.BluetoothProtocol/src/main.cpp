/**
 * @file main.cpp
 * @author IoT's Students
 * @brief A simple protocol used to manage a communication between two semaphores.
 * 
 * @version 0.1
 * @date 2022-11-18
 * 
 * @copyright Copyright (c) 2022
 */

#include <Arduino.h>
#include <BluetoothSerial.h>

// It is used to discriminate if a device is a slave or a master.
#define isSlave true

// It is the name of the bluetooth device.
#define blname "Semaforo"

// These are the types of messages that can be sent.
#define DATA 0
#define ACK 1

// These are the pins used to control the semaphores.
#define RED 18
#define YELLOW 21
#define GREEN 23

// These are the states of the semaphores.
#define R 0
#define Y 1
#define G 2
#define W 3

// These are sender and receiver addresses.
#define SENDER "30:ae:a4:eb:92:01"
#define RECEIVER "30:ae:a4:eb:92:02"

// Object for Bluetooth communication.
BluetoothSerial SerialBT;

esp_spp_sec_t sec_mask = ESP_SPP_SEC_NONE; // or ESP_SPP_SEC_ENCRYPT|ESP_SPP_SEC_AUTHENTICATE to request pincode confirmation
esp_spp_role_t role = ESP_SPP_ROLE_SLAVE; // or ESP_SPP_ROLE_MASTER

// It is used to store the state of the semaphore.
uint8_t semaphore_state = W; 

// This is the struct of the packet.
struct Generic_BLPacket {
  uint16_t id;
  uint8_t type;
  char dest[19];
  char sender[19];
  uint8_t payload [33];
};

// This is an example of payload.
String sendData = "PAYLOAD";

// This is the packet that will be sent.
Generic_BLPacket pacchetto;

// This is the packet that will be received.
Generic_BLPacket pacchettoRicevuto;

// This is the function that will be called when a packet is received.
bool send(Generic_BLPacket *packet){
  int sentBytes = SerialBT.write((uint8_t *)packet, sizeof(Generic_BLPacket));
  if(sentBytes == sizeof(*packet)) return true;
  return false;

}

// This is the function that will be called in order to send an ACK.
bool sendACK(int id) {
  Generic_BLPacket pacchetto;
  pacchetto.id = id;
  pacchetto.type = ACK;
  strcpy(pacchetto.dest, RECEIVER);
  strcpy(pacchetto.sender, SENDER);
  memcpy(pacchetto.payload, sendData.c_str(), sendData.length()+1);
  int sentBytes = SerialBT.write((uint8_t *)(&pacchetto), sizeof(Generic_BLPacket));
  if(sentBytes == sizeof(&pacchetto)) return true;
  return false;
}


// This is the function that will be called when a packet is received.
void callback_BT_data(const uint8_t *buffer, size_t size) {
    memcpy(&pacchettoRicevuto, buffer, size);
    Serial.println("Received:");
    Serial.printf("tx: %s\n", pacchettoRicevuto.payload);
    if(pacchettoRicevuto.type == DATA) {
      sendACK(pacchettoRicevuto.id);
      semaphore_state = G;
      digitalWrite(RED, 0); 
      digitalWrite(YELLOW, 0); 
      digitalWrite(GREEN, 1); 
    }
    else Serial.printf("ACK received\n");
}

// This is the setup function.
void setup() {
    pinMode(RED, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(GREEN, OUTPUT);
    
    digitalWrite(RED, 1);
    digitalWrite(YELLOW, 0);
    digitalWrite(GREEN, 0);

    Serial.begin(115200); // Start Serial monitor in 9600
    if(isSlave) {
       SerialBT.begin(blname);
    }
    else {
      SerialBT.begin(blname, true); // Name of your Bluetooth Signal
      Serial.println("Bluetooth Device is Ready to Pair");
      BTScanResults *btDeviceList = SerialBT.discover(5000);
      if (btDeviceList->getCount() > 0) {
        BTAddress addr;
        int channel = 0;
        Serial.println("Found devices:");

        for (int i = 0; i < btDeviceList->getCount(); i++) {
          BTAdvertisedDevice *device = btDeviceList->getDevice(i);
          Serial.printf(" ----- %s  %s %d\n", device->getAddress().toString().c_str(), device->getName().c_str(), device->getRSSI());
          if (device->getName() == blname) {
            Serial.println("Sem1 founded!");
            Serial.printf("Connecting to %s \n", blname);
            if(SerialBT.connect(blname)) {
              Serial.println("Connected!");
              break;
            } 
            else {
              Serial.println("Restart in 2 seconds");
              delay(2000);
              ESP.restart();
            }
          }
        }

        if(!SerialBT.connected(20)) {
          Serial.println("Timeout connection");
          Serial.println("Restart in 2 seconds");
          delay(2000);
          ESP.restart();
        }
      }
      else
      {
          Serial.println("Didn't find any devices");
          Serial.println("Restart in 2 seconds");
          delay(2000);
          ESP.restart();
      }
    }
    SerialBT.onData(callback_BT_data);
}

// This is the loop function.
void loop() {
  if (semaphore_state == G) {
    delay(2000); 
    semaphore_state = Y;
    digitalWrite(RED, 0); 
    digitalWrite(YELLOW, 1); 
    digitalWrite(GREEN, 0);

  }
  else if (semaphore_state == Y) {
    delay(1000); 
    semaphore_state = R; 
    digitalWrite(RED, 1); 
    digitalWrite(YELLOW, 0); 
    digitalWrite(GREEN, 0); 

  }
  else if (semaphore_state == R) {
    semaphore_state = W; 
    pacchetto.id = 0;
    pacchetto.type = DATA;
    strcpy(pacchetto.dest, RECEIVER);
    strcpy(pacchetto.sender, SENDER);
    memcpy(pacchetto.payload, sendData.c_str(), sendData.length()+1);
    send(&pacchetto);
  }
  else {
    Serial.println("Waiting..."); 
  }
}