/***
 * @author: 	Luigi Seminara
 * @date:		16-12-2022
 * @version:	0.1
 * @brief:      This is a code use to test the ARSLab-Devkit
*/

#include <SPI.h>
#include <mrf24j.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_SSD1306.h> // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Wire.h>

// These are the pins used of the ESP32
#define PIN_WHITE   13
#define PIN_BLUE    12
#define SEM_RED     26
#define SEM_YELLOW  27 
#define SEM_GREEN   14
#define PIN_BTN1    25
#define PIN_BTN2    33
#define PIN_RESET   2
#define PIN_CS      5
#define PIN_INT     4

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// Create the display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create the mrf24j object
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
int currentBtnState[2] = {digitalRead(PIN_BTN1), digitalRead(PIN_BTN2)};  

void setup() {
    Serial.begin(115200);

    pinMode(PIN_WHITE, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(SEM_RED, OUTPUT);
    pinMode(SEM_YELLOW, OUTPUT);
    pinMode(SEM_GREEN, OUTPUT);
    pinMode(PIN_BTN1, INPUT);
    pinMode(PIN_BTN2, INPUT);

    digitalWrite(PIN_WHITE, 0);
    digitalWrite(PIN_BLUE, 1);

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

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(2000); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    // Display static text
    display.println("Hello!");

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();
    delay(2000);
}

void loop() {
    mrf.check_flags(&handle_rx, &handle_tx);
    unsigned long current_time = millis();
    if (current_time - last_time > tx_interval) {
        last_time = current_time;
        
        char buf[] = "6001";
        mrf.send16(0x6002, buf);

        digitalWrite(PIN_WHITE, digitalRead(PIN_BLUE));
        digitalWrite(PIN_BLUE, !digitalRead(PIN_WHITE));
        digitalWrite(SEM_RED, !digitalRead(SEM_RED));
        digitalWrite(SEM_YELLOW, !digitalRead(SEM_YELLOW));
        digitalWrite(SEM_GREEN, !digitalRead(SEM_GREEN));

        if (!currentBtnState[0] == HIGH) {
            Serial.println("Button 1 pressed");
        } else {
            Serial.println("Button 1 released");
        }
        currentBtnState[0] = digitalRead(PIN_BTN1);

        if (!currentBtnState[1] == HIGH) {
            Serial.println("Button 2 pressed");
        } else {
            Serial.println("Button 2 released");
        }
        currentBtnState[1] = digitalRead(PIN_BTN2);
    }
}