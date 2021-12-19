## Note: This project is not maintained anymore. 
**\*If\*** the developer decides to continue this project, Gui900 will be created from scratch with a new architecture, and possibly under a different name. 

# Gui900

Using this library, you can make a nice UI for your application (which is running on an Arduino board) in just a few minutes. You can make a better interface with a more readable code, and save time and energy.  
Just say how your app should look like and what should it do, and Gui900 will take care of the rest.

## Features

* **Support for multi-page programs**
  
  You can add as many as 32767 pages if you have enough RAM
* **Support for button, label, check-box, slider and radio-button**
* **Flexibility**
  
  You can customize almost everything of the elements.  
  There are a lot of optional things.
* **Screen time-out**
  
  TFT screen will turn off after inactivity to reduce power usage and avoid image persistance (optional)
* **Ease of use**
  
  Almost every action only needs one function call, and some of them are automatically done.  
  And only one configuration is needed for most of the modules (Multi-page, buttons, check-boxes, sliders, radio-buttons, screen-timeout, screen brightness, fonts, screen-off animation, touch)
* **Visual editor**

  [Gui900 visual editor](https://mammad900.github.io/Gui900-visual-editor/) makes writing Gui900 apps much faster and easier.  
  It generates most of Gui900 related code automatically and lets you preview the app without using a real Arduino board.

## Instructions

*Note: You don't need to do these if you use [Gui900 visual editor](https://mammad900.github.io/Gui900-visual-editor/)*

To create an application using this GUI library, you have to:

1. Open Arduino IDE and install MCUFRIEND_kbv , Adafruit GFX and Adafruit-touchscreen using library manager
2. Create a new sketch
3. In the new sketch, `#include` the library file (library.ino)
4. Copy the contents of config.ino to your sketch
5. Adjust the constants you copied from config.ino specific to your program (it's heavily commented and easy to use)

* In setup():

6. Build the pages using [`addbutton()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/addbutton()), [`addlabel()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/addlabel()), [`addcheckBox()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/addcheckBox()), [`addslider()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/addslider()) and `addradioButton()`
7. Call [`start()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/start()) to initialize the TFT screen and render the first page

* In loop():

8. Call [`checkPage()`](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/checkPage()) to make the application responsive to touches
9. Check element states and respond to changes if needed

**[Reference](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki#reference)**

This library uses MCUFIRIEND_kbv (available in library manager) as TFT library but will work with most libraries using Adafruit GFX.
TFT controller-specific commands (just 3) are written for ILI9325 (but may work on some other controllers).

Note: To compile this library, make sure to use the `-fpermissive` flag or it won't compile. You have to pass `-fpermissive` explicitly if you want to use this library on Arduino Due or you are using an old version of the Arduino IDE.

If you have any questions, take a look at [FAQ](https://github.com/Mammad900/Arduino-TFT-GUI-library/wiki/Frequently-Asked-Questions)
