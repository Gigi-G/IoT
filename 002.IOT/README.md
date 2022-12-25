# IOT Library

`./lib/IOT` contains the definition of the `IOT` class that define some functionality related to interacting with an MRF24J module and an OLED display SSD1306. The class has a number of private member variables, including an instance of a `Mrf24j` class and an instance of an `Adafruit_SSD1306` class.

The class has a number of private member functions, including `initDisplay`, `initMrf24j`, and `init`, which are used to initialize the display and the MRF24J module.

The class has a number of public member functions, including `getMrf` and `getDisplay`, which return the `Mrf24j` and `Adafruit_SSD1306` objects, respectively. There is also a `check_flags` function which take two function pointers as arguments and invokes these functions when certain events occur. Finally, there is a `drawText` function which takes a string, along with some positional and size arguments, and it is used to draw text on the OLED display.

There are a few additional details that can be inferred about the `IOT` class:

- The `Mrf24j` class is a class that represents an MRF24J module, and it provides functionality for interacting with the module, such as sending and receiving packets.
- The `Adafruit_SSD1306` class is a class provided by the Adafruit library for interacting with OLED displays. It provides functions for drawing text and graphics on the display, as well as controlling the display's brightness and other settings.
- The `initDisplay` function is used to initialize the OLED display and prepare it for use.
- The `initMrf24j` function is used to initialize the MRF24J module and configure its various settings, such as the PAN ID, address, and whether it should operate in promiscuous mode.
- The `init` function is a wrapper function that calls both `initDisplay` and `initMrf24j`.
- The `check_flags` function is likely used to check for certain events or conditions and invoke the provided handler functions when the MRF24J either receives or sends data.
- The `drawText` function is used to draw a given string of text on the OLED display at the specified position and with the specified size.

