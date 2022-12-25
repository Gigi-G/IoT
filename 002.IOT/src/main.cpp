#include <IOT.h>

IOT* iot;
long last_time;
long tx_interval = 1000;
bool btn1Pressed = false;
bool btn2Pressed = false;

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
	btn1Pressed = true;
}


void IRAM_ATTR button2_isr() {
	btn2Pressed = true;
}


void interrupt_routine() {
  if(iot) iot->getMrf().interrupt_handler(); 
}
#pragma endregion

void setup() {
  Serial.begin(115200);
  iot = new IOT(0xcafe, 0x6001, false, false, false);

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

  if(btn1Pressed) {
    iot->drawText("Button 1 pressed", 0, 0, 1);
    btn1Pressed = false;
  }

  if(btn2Pressed) {
    iot->drawText("Button 2 pressed", 0, 0, 1);
    btn2Pressed = false;
  }
}