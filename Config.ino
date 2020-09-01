// Number of pages
// Default: 10
#define PAGES 6

// The screen time-out in seconds
// When user does not touch anywhere on the screen for more than this value, the display will turn off
// Comment this to disable the feature
#define SCREENTIMEOUT 30

// The screen brightness that is controlled by BACKLIGHT_PIN
// Use changeBrightness() to dinamically change brightness
byte brightness=255;

// Fonts will fill a lot of flash memory (up to 40KB)
// So apps written with this library won't fit in small flash memories.
// Uncomment this to disable FreeSans fonts and use NULL instead,
// this releases a lot of program memory. (EXPERIMENTAL)
//#define DISABLEFONTS

// If you uncomment this, disabled elements will not be dimmed
//#define DONOTDIMDISABLED

// Uncomment this to keep screen on/off code when screen timeout is disabled
// By default, when you comment SCREENTIMEOUT, the scrOn() and scrOff() methods will become unavailable.
// If you uncomment this, those methods will remain available.
//#define ALLOWSCREENCONTROL

// The screen off animation time in millis
// Total animation time will be 256 times more
// Comment this to disable animation on screen time-out.
// I suggest not to disable it, because a white screen will appear for a short time, depending on the amount of time the screen was on.
// I tried to add a delay() between turning off backlight and putting the TFT screen to sleep, but it didn't work.
#define SCROFFANITIME 2

// Background color of pages
// An array of 16-bit color values
uint16_t page_backColors[PAGES]{0,0,0,0,0,0};

//The pin to control tft backlight
//It is recommended to use a PWM pin.
//If you use an UNO shield on a mega2560, pins 46, 47, 48 are recommended
//Just remove/comment it if you don't have such a pin
#define BACKLIGHT_PIN 46

// Maximum buttons in a page. Too low value causes (miserable) bugs, too much causes high SRAM usage.
// e.g. if you have 3 pages: one with 3 buttons, one with 5 buttons, one with 4 buttons. You should choose 5.
// Comment this if you don't use any buttons
#define BUTTON 9

// Maximum labels in a page.
// Comment this if you don't use any labels
#define LABEL 3

// Maximum check boxes in a page.
// Comment this if you don't use any check boxes
#define CHECKBOX 1

// Maximum sliders in a page.
// Comment this if you don't use any sliders
#define SLIDER 1

// Orientation of the TFT
// 0:Portrait (0°)
// 1:Landscape(90°)
// 2:Portrait reverse (180°)
// 3:Landscape reverse (270°)
// Default: Portrait
#define Orientation 0

// Maximum and minimum pressure needed to instigate touch
// Edit this only if you are familiar with Adafruit touch screen library
// They are optional
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// Pin and calibration constants for touch screen. Use TouchScreen_Calibr_Native
// to find out your own values.
// If you have a fractured touch screen (like mine), TouchScreen_Calibr_Native returns false values. You will have to investigate yourself.
const int TS_LEFT = 950, TS_RT = 100, TS_TOP = 100, TS_BOT = 944, XP = 6, XM = A2, YP = A1, YM = 7;
