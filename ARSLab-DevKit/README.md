# ARSLab-Devkit

The ESP32 is a low-cost, low-power microcontroller with built-in Wi-Fi and Bluetooth connectivity. It is often used in Internet of Things (IoT) projects and can be programmed using the Arduino Integrated Development Environment (IDE) or other software development tools.

To use buttons, LEDs, and an OLED display SSD1306 with the ESP32, you can follow this electrical scheme:

1. The OLED display has a set of pins for power, ground, and data communication (I2C). These pins should be connected to corresponding pins on the ESP32.
2. The MRF24J module has a set of pins for power, ground, and data communication (SPI). These pins should be connected to corresponding pins on the ESP32.
3. The buttons can be connected to digital input pins on the ESP32, and a pull-down resistor should be used to ensure a stable input when the button is not pressed and a RC filter in order to reduce the **debouncing**.
4. The LEDs can be connected to digital output pins on the ESP32, and a current-limiting resistor should be used to prevent damage to the LED or the ESP32.

Here is a table of the pin mapping for the buttons, LEDs, and OLED SSD1306 display:

<table>
    <thead><tr><th>Device</th><th>ESP32 Pin</th></tr></thead>
    <tbody>
        <tr>
            <td>PIN_BTN1</td>
            <td>25</td>
        </tr>
        <tr>
            <td>PIN_BTN2</td>
            <td>33</td>
        </tr>
        <tr>
            <td>PIN_WHITE</td>
            <td>13</td>
        </tr>
        <tr>
            <td>PIN_BLUE</td>
            <td>12</td>
        </tr>
        <tr>
            <td>SEM_RED</td>
            <td>26</td>
        </tr>
        <tr>
            <td>SEM_YELLOW</td>
            <td>27</td>
        </tr>
        <tr>
            <td>SEM_GREEN</td>
            <td>14</td>
        </tr>
        <tr>
            <td>PIN_RESET</td>
            <td>2</td>
        </tr>
        <tr>
            <td>PIN_CS</td>
            <td>5</td>
        </tr>
        <tr>
            <td>PIN_INT</td>
            <td>4</td>
        </tr>
    </tbody>
</table>

