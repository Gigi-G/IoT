/***
 * @author: 	Luigi Seminara
 * @date:		16-12-2022
 * @version:	0.1
 * @brief:      This is a code use to test the ARSLab-Devkit
*/

#include <SPI.h>
#include <mrf24j.h>

// These are the pins used of the ESP32
#define PIN_BLUE    27
#define PIN_GREEN   26
#define SEM_RED     32
#define SEM_YELLOW  33 
#define SEM_GREEN   35
#define PIN_RESET   2
#define PIN_CS      5
#define PIN_INT     4

Mrf24j mrf(PIN_RESET, PIN_CS, PIN_INT);

/**
 * This function is called when an interrupt is triggered
*/
void interrupt_routine() {
    // mrf24 object interrupt routine
    mrf.interrupt_handler();
}

/**
 * This function is called when a packet is received
*/
void handle_rx() {
    Serial.print("received a packet ");Serial.print(mrf.get_rxinfo()->frame_length, DEC);Serial.println(" bytes long");
    
    if(mrf.get_bufferPHY()){
      Serial.println("Packet data (PHY Payload):");
      for (int i = 0; i < mrf.get_rxinfo()->frame_length; i++) {
          Serial.print(mrf.get_rxbuf()[i]);
      }
    }
    
    Serial.println("\r\nASCII data (relevant data):");
    for (int i = 0; i < mrf.rx_datalength(); i++) {
        Serial.write(mrf.get_rxinfo()->rx_data[i]);
    }
    
    Serial.print("\r\nLQI/RSSI=");
    Serial.print(mrf.get_rxinfo()->lqi, DEC);
    Serial.print("/");
    Serial.println(mrf.get_rxinfo()->rssi, DEC);
}

/**
 * This function is called when a packet is sent
*/
void handle_tx() {
    if (mrf.get_txinfo()->tx_ok) {
        Serial.println("TX went ok, got ack");
    } else {
        Serial.print("TX failed after ");Serial.print(mrf.get_txinfo()->retries);Serial.println(" retries\n");
    }
}

long last_time;
long tx_interval = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(SEM_RED, OUTPUT);
  pinMode(SEM_YELLOW, OUTPUT);
  pinMode(SEM_GREEN, OUTPUT);

  digitalWrite(PIN_BLUE, 0);
  digitalWrite(PIN_GREEN, 1);

  digitalWrite(SEM_RED, 0);
  digitalWrite(SEM_YELLOW, 0);
  digitalWrite(SEM_GREEN, 0);
  
  mrf.reset();
  mrf.init();
  
  mrf.set_pan(0xcafe);

  // This is _our_ address
  mrf.address16_write(0x6001); 

  // uncomment if you want to receive any packet on this channel
  //mrf.set_promiscuous(true);
  
  // uncomment if you want to enable PA/LNA external control
  //mrf.set_palna(true);
  
  // uncomment if you want to buffer all PHY Payload
  //mrf.set_bufferPHY(true);

  attachInterrupt(PIN_INT, interrupt_routine, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
  last_time = millis();
  interrupts();
}

void loop() {
    mrf.check_flags(&handle_rx, &handle_tx);
    unsigned long current_time = millis();
    if (current_time - last_time > tx_interval) {
        last_time = current_time;
        
        char buf[] = "6001";
        mrf.send16(0x6002, buf);

        digitalWrite(PIN_BLUE, digitalRead(PIN_GREEN));
        digitalWrite(PIN_GREEN, !digitalRead(PIN_GREEN));
        digitalWrite(SEM_RED, !digitalRead(SEM_RED));
        digitalWrite(SEM_YELLOW, !digitalRead(SEM_YELLOW));
        digitalWrite(SEM_GREEN, !digitalRead(SEM_GREEN));
    }
}