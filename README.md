# Arduino TFT GUI library
Allows you to create a multi-page program containing buttons, labels, check-boxes and sliders.

## Instructions
To create an application using this GUI library, you should:
1. `#include` the library file (library.ino)
2. Copy the contents of config.ino to your sketch
3. Adjust the constants you copied from config.ino specific to your program (it's heavily commented and easy to use)
* In setup():
4. Build the pages using [`addbutton()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/addbutton()), `addlabel()`, `addcheckBox()` and `addslider()`
5. Call [`start()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/start()) to initialize the TFT screen and render the first page
* In loop():
6. Call [`checkPage()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/checkPage()) to make the application responsive to touches
7. Check element states and respond to changes if needed

This library uses MCUFIRIEND_kbv (available in library manager) as TFT library but will work with most libraries using Adafruit GFX.
TFT controller-specific commands (just 3) are written for ILI9325 (but may work on some other controllers).

Note: To compile this library, make sure to use the `-fpermissive` flag or it won't compile. You have to pass `-fpermissive` explicitly if you want to use this library on Arduino Due or you are using an old version of the Arduino IDE.

**This project is still under development**
