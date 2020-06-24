# Arduino TFT GUI library
Enables you to create a multi-page program containing buttons, labels, check-boxes and sliders.

## Instructions
1. Reference the library file (library.ino) using a #include
2. Copy the contents of config.ino to your sketch
3. Adjust the constants you copied from config.ino specific to your program
4. Use the API to build the program UI and control it

The default library is MCUFRIEND_kbv but will work with most adafruit GFX libraries.
TFT controller-specific commands (just 3) are written for ILI9325 (but may work on some other controllers).
