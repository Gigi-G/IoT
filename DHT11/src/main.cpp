#include <IOT.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11     // DHT 11
#define DHTPIN  15        // Digital pin connected to the DHT sensor

IOT* iot;

long last_time;

long tx_interval = 1000;

DHT_Unified dht(DHTPIN, DHTTYPE);

enum State {NONE, TEMP, HUMIDITY};

State state = NONE;

/**
 * @brief This are example handlers, you can write your own
*/
#pragma region Handlers
void handle_rx() {
  if(!iot) return;
  Serial.print("received a packet ");
  Serial.print(iot->getMrf().get_rxinfo()->frame_length, DEC);
  Serial.println(" bytes long");
  
  if(iot->getMrf().get_bufferPHY()){
    Serial.println("Packet data (PHY Payload):");
    for (int i = 0; i < iot->getMrf().get_rxinfo()->frame_length; i++) {
      Serial.print(iot->getMrf().get_rxbuf()[i]);
    }
  }
  
  Serial.println("\r\nASCII data (relevant data):");
  for (int i = 0; i < iot->getMrf().rx_datalength(); i++) {
    Serial.write(iot->getMrf().get_rxinfo()->rx_data[i]);
  }
  
  Serial.print("\r\nLQI/RSSI=");
  Serial.print(iot->getMrf().get_rxinfo()->lqi, DEC);
  Serial.print("/");
  Serial.println(iot->getMrf().get_rxinfo()->rssi, DEC);
}


void handle_tx() {
  if(!iot) return;
  if (iot->getMrf().get_txinfo()->tx_ok) {
      Serial.println("TX went ok, got ack");
  } else {
      Serial.print("TX failed after ");
      Serial.print(iot->getMrf().get_txinfo()->retries);
      Serial.println(" retries\n");
  }
}


void IRAM_ATTR button1_isr() {
	state = TEMP;
}


void IRAM_ATTR button2_isr() {
	state = HUMIDITY;
}


void interrupt_routine() {
  if(iot) iot->getMrf().interrupt_handler(); 
}
#pragma endregion

void setup() {
  Serial.begin(115200);
  iot = new IOT(0xcafe, 0x6001, false, false, false);
  dht.begin();

  // attach interrupt
  attachInterrupt(PIN_BTN1, button1_isr, FALLING);
  attachInterrupt(PIN_BTN2, button2_isr, FALLING);
  attachInterrupt(PIN_INT, interrupt_routine, CHANGE);
  last_time = millis();
  interrupts();
}


void loop() {
  iot->check_flags(&handle_rx, &handle_tx);
  unsigned long current_time = millis();

  if (current_time - last_time > tx_interval) {
      last_time = current_time;
      char buf[] = "6001";
      iot->getMrf().send16(0x6002, buf);
  }

  sensors_event_t event;

  if(state == TEMP) {
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      iot->drawText(F("Error reading temperature!"), 0, 0, 2);
    }
    else {
      char buffer[256];
      sprintf(buffer, "T: %.2f\nDegrees \nCelsius", event.temperature);
      iot->drawText(buffer, 0, 0, 2);
    }
  }

  if(state == HUMIDITY) {
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      iot->drawText(F("Error reading humidity!"), 0, 0, 2);
    }
    else {
      char buffer[256];
      sprintf(buffer, "H: %.2f\npercentage", event.relative_humidity);
      iot->drawText(buffer, 0, 0, 2);
    }
  }
  delay(1000);
}