#include <IOT.h>


IOT::IOT(word panid, word address, bool promiscuous, bool palna, bool bufferPHY) : 
        mrf(PIN_RESET, PIN_CS, PIN_INT), 
        display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {

    // Initialize the pins
    pinMode(PIN_WHITE, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_YELLOW, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BTN1, INPUT_PULLUP);
    pinMode(PIN_BTN2, INPUT_PULLUP);

    // Turn off the LEDs
    digitalWrite(PIN_WHITE, 0);
    digitalWrite(PIN_BLUE, 0);
    digitalWrite(PIN_RED, 0);
    digitalWrite(PIN_YELLOW, 0);
    digitalWrite(PIN_GREEN, 0);

    // Initialize the display and the mrf24j
    init(panid, address, promiscuous, palna, bufferPHY);
}


void IOT::init(word panid, word address, bool promiscuous, bool palna, bool bufferPHY) {
    initDisplay();
    initMrf24j(panid, address, promiscuous, palna, bufferPHY);
}


void IOT::initDisplay() {
    // Initialize the display
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }

    // Clear the display
    display.clearDisplay();
    display.display();

    // Set the text size
    display.setTextSize(2);

    // Set the text color
    display.setTextColor(WHITE);

    // Set the cursor
    display.setCursor(0, 0);

    // Print the text
    display.println(F("IOT"));
    display.println(F("Project"));
    display.println(F("University"));
    display.println(F("of Catania"));

    // Display the text
    display.display();
}


void IOT::initMrf24j(word panid, word address, bool promiscuous, bool palna, bool bufferPHY) {
    mrf.reset();
    mrf.init();

    mrf.set_pan(panid);

    // This is _our_ address
    mrf.address16_write(address); 

    // uncomment if you want to receive any packet on this channel
    mrf.set_promiscuous(promiscuous);

    // uncomment if you want to enable PA/LNA external control
    mrf.set_palna(palna);

    // uncomment if you want to buffer all PHY Payload
    mrf.set_bufferPHY(bufferPHY);
}


void IOT::check_flags(void (*handle_tx)(), void (*handle_rx)()) {
    mrf.check_flags(handle_tx, handle_rx);
}

void IOT::drawText(String text, int x, int y, int size) {
    // Clear the display
    display.clearDisplay();
    display.display();

    // Set the text size
    display.setTextSize(size);

    // Set the cursor
    display.setCursor(x, y);

    // Set the text color
    display.setTextColor(WHITE);

    // Print the text
    display.println(text);

    // Display the text
    display.display();
}