/**
 * @file        IOT.h
 * @author      Luigi Seminara
 * @brief       This is the library for the IOT project @ University of Catania
 * 
 * @version     0.1
 * @date        2022-12-24
 * 
 * @copyright   Copyright (c) 2022
 */

#include <SPI.h>                                // SPI
#include <mrf24j.h>                             // MRF24J40
#include <Adafruit_GFX.h>                       // Core graphics library
#include <Adafruit_SSD1306.h>                   // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>    // Font
#include <Wire.h>                               // I2C

// These are the pins used of the ESP32
#define PIN_WHITE   13      // White LED
#define PIN_BLUE    12      // Blue LED
#define PIN_RED     26      // Red LED
#define PIN_YELLOW  27      // Yellow LED
#define PIN_GREEN   14      // Green LED
#define PIN_BTN1    25      // Button 1
#define PIN_BTN2    33      // Button 2
#define PIN_RESET   2       // Reset pin
#define PIN_CS      5       // Chip select pin
#define PIN_INT     4       // Interrupt pin

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels

#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)

class IOT {
    private:
        // MRF24J40
        Mrf24j mrf;

        // OLED
        Adafruit_SSD1306 display;

        /**
         * @brief Initialize the display
        */
        void initDisplay();

        /**
         * @brief Initialize the MRF24J40
         * @param panid         PAN ID
         * @param address       Address
         * @param promiscuous   Promiscuous mode
         * @param palna         PALNA
         * @param bufferPHY     BufferPHY
        */
        void initMrf24j(word panid, word address, bool promiscuous, bool palna, bool bufferPHY);

        /**
         * @brief Initialize the IOT object
         * @param panid         PAN ID
         * @param address       Address
         * @param promiscuous   Promiscuous mode
         * @param palna         PALNA
         * @param bufferPHY     BufferPHY
        */
        void init(word panid, word address, bool promiscuous, bool palna, bool bufferPHY);
    
    public:

        /**
         * @brief Construct a new IOT object
         * @param panid         PAN ID
         * @param address       Address
         * @param promiscuous   Promiscuous mode
         * @param palna         PALNA
         * @param bufferPHY     BufferPHY
        */
        IOT(word panid, word address, bool promiscuous, bool palna, bool bufferPHY);

        /**
         * @brief Return the MRF24J40 object
        */
        Mrf24j getMrf() { return mrf; };
        
        /**
         * @brief Return the display object
        */
        Adafruit_SSD1306 getDisplay() { return display; };

        /**
         * @brief Call this function periodically, it will invoke your nominated handlers
         * @param handle_tx     This function is called when a packet is sent
         * @param handle_rx     This function is called when a packet is received
        */
        void check_flags(void (*handle_tx)(), void (*handle_rx)());

        /**
         * @brief Draw a text on the display
         * @param text  Text to draw
         * @param x     X position
         * @param y     Y position
         * @param size  Size of the text
        */
        void drawText(String text, int x, int y, int size);
};