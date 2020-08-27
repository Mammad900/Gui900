# Arduino TFT GUI library

Using this library, you can make an application with a nice UI in just a few minutes. You can make a better interface with a more readable code, and save time and energy.  
Just say how your app should look like and what should it do, and the library will take care of the rest.

## Features

* **Support for multi-page programs**  
  You can add as many as 32767 pages
* **Support for button, label, check-box and slider**
* **Ability to include another sketch in the program (experimental)**  
  Just paste the code of the second sketch, rename the functions and variables to avoid conflicts, and call `runsketch(setup,loop)`
* **Flexibility**  
  You can customize almost everything of the elements.  
  There are a lot of optional things.
* **Automatic font size for buttons**  
  Font size will automatically calculated to fit into the button
* **Screen time-out**  
  TFT screen will turn off after inactivity to reduce power usage and avoid image persistance (optional)
* **Ease of use**  
  Almost every action only needs one function call, and some of them are automatically done.  
  And only one configuration is needed for most of the modules (Multi-page, buttons, check-boxes, sliders, screen-timeout, screen brightness, fonts, screen-off animation, touch)

## Instructions

To create an application using this GUI library, you have to:

1. Open Arduino IDE and install MCUFRIEND_kbv , Adafruit GFX and Adafruit-touchscreen using library manager
2. Create a new sketch
3. In the new sketch, `#include` the library file (library.ino)
4. Copy the contents of config.ino to your sketch
5. Adjust the constants you copied from config.ino specific to your program (it's heavily commented and easy to use)

* In setup():

1. Build the pages using [`addbutton()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/addbutton()), [`addlabel()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/addlabel()), [`addcheckBox()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/addcheckBox()) and [`addslider()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/addslider())
2. Call [`start()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/start()) to initialize the TFT screen and render the first page

* In loop():

1. Call [`checkPage()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/checkPage()) to make the application responsive to touches
2. Check element states and respond to changes if needed

**[Reference](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki#reference)**

This library uses MCUFIRIEND_kbv (available in library manager) as TFT library but will work with most libraries using Adafruit GFX.
TFT controller-specific commands (just 3) are written for ILI9325 (but may work on some other controllers).

Note: To compile this library, make sure to use the `-fpermissive` flag or it won't compile. You have to pass `-fpermissive` explicitly if you want to use this library on Arduino Due or you are using an old version of the Arduino IDE.

If you have any questions, take a look at [FAQ](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/Frequently-Asked-Questions)
