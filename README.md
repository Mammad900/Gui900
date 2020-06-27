# Arduino TFT GUI library
Enables you to create a multi-page program containing buttons, labels, check-boxes and sliders.

## Instructions
* In global scope:
To make an application using this library, you have to:
1. Reference the library file (library.ino) using a #include
2. Copy the contents of config.ino to your sketch
3. Adjust the constants you copied from config.ino specific to your program
* In setup():
4. Build the pages using `addbutton()`, `addlabel()`, `addcheckBox()` and `addslider`
5. Call `start()` to init the TFT screen and render the first page
* In loop():
6. Call `checkPage()` to make the application responsive
7. Check element states and respond to changes if needed

The default library is MCUFRIEND_kbv but will work with most adafruit GFX libraries.
TFT controller-specific commands (just 3) are written for ILI9325 (but may work on some other controllers).

Note: When compiling this library, make sure to pass the `-fpermissive` flag to the compiler or it won't compile. You have to pass the `-fpermissive` explicitly if you want to compile for DUE or have an old IDE.
