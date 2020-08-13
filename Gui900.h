#ifndef __GUI900_H__
#define __GUI900_H__

/*
  Gui900.h - Library for creating an interface for applications using a TFT screen.
  Created by Mohammad Hossein Abedi, July 29, 2020.
  Released into the public domain.
*/

#include <Arduino.h>

//Libraries
    #include <Adafruit_GFX.h>                       // Core graphics library, dependency of MCUFRIEND_kbv
    #include <MCUFRIEND_kbv.h>                      // Hardware-specific library
    #include <TouchScreen.h>                        // Touch library
    #include <FreeDefaultFonts.h>                   // Some normal fonts
    #include <Fonts/FreeSans9pt7b.h>                // Small Sans font
    #include <Fonts/FreeSans12pt7b.h>               // Normal Sans font
    #include <Fonts/FreeSans18pt7b.h>               // Big Sans font
    #include <Fonts/FreeSans24pt7b.h>               // Huge Sans font
    
//Defines
    #define initArray1(arr,s) arr=initializeArray(arr,s)
    #define initArray2(arr,s1,s2) arr=initialize2dArray(arr,s1,s2)
    #define runsketch(stp,lop) stp();while(true){lop();} // Runs an embedded sketch
    #define HCT haschanged=true; // This is the [famous] macro that makes the library aware that something has changed.
        #define XPOS 1          // X position
        #define YPOS 2          // Y position
        #define HEIGHT 3        // Height
        #define WIDTH 4         // Width
        #define TEXT 5          // Button label
        #define TEXT_COLOR 6    // Text (foreground) color
        #define BACK_COLOR 7    // Background color
        #define BORDER_COLOR 8  // Border color
        #define ENABLED 9       // Enabled
        #define VISIBLE 10      // Visiblity

        #define CIRCLE 9996     // An AUTO for radius
    #define CENTER 9998 // When used, position will be dynamically calculated to align the label to the center
    #define RIGHT 9999  // When used, position will be dynamically calculated to align the label to the right or bottom
        #define SIZE9PT18PX 18  // Use 18 pixels height and FreeSans9pt7b
        #define SIZE12PT28PX 28 // Use 28 pixels height and FreeSans12pt7b
        #define SIZE18PT42PX 42 // Use 42 pixels height and FreeSans18pt7b
        #define SIZE24PT56PX 56 // Use 56 pixels height and FreeSans24pt7b
    #define DEFAULT 1   // Default transition for navigation between pages
    #define AUTO 9997   // Used for automatic values. They will be dynamically changed.

    
class Gui900{
    private:
        byte brightness;
        int _backlightPin;
        bool haschanged=false; // Has anything changed since the screen was off?
        int XM,YM,XP,YP,minPressure,maxPressure;
        int tsLeft,tsRight,tsTop,tsBottom;
        int orientation;
        int timeOut;
        int pageCount,buttonCount,labelCount,checkBoxCount,sliderCount;
		uint16_t** initialize2dArray(uint16_t** arr, uint16_t d1, uint16_t d2);
		uint16_t* initializeArray(uint16_t* arr, uint16_t size);
		String** initialize2dArray(String** arr, uint16_t d1, uint16_t d2);
		String* initializeArray(String* arr, uint16_t size);
		bool** initialize2dArray(bool** arr, uint16_t d1, uint16_t d2);
		bool* initializeArray(bool* arr, uint16_t size);
        void initarr();
//Function prototypes 
public:
    MCUFRIEND_kbv tft; //The TFT screen object
    TouchScreen ts=TouchScreen(0,0,0,0,300) ;  // The touch screen object
bool quit; // Used for runsketch()
    Gui900(int numPages,int numButtons, int numLabels, int numCheckBoxes, int numSliders);
    void configTouch(int XP,int XM, int YP, int YM, int RX=300);
    void calibrateTouch(int left,int right, int top, int bottom, int min, int max);
    void timeOutEnable(int timeout);
    void(* Reset) (void) = 0;   // Soft-resets Arduino microcontroller
    bool Touch_getXY(void);     // Reads touch. If touch is detected, returns true and assigns pixel_x , pixel_y and pixel_z variables to touched coordinates
    int Centre(int Length, int wid = AUTO, int Left = 0, int CHwid = 6); //Calculates the position needed to align an object to the centre
    bool inRegion(int Y, int Bot, int Top = 0, int X = 120, int Left = 0, int Right = 240); //If given coordinates are in the given range, returns true.
    int calculateListY(int num); // Calculates the Y position of a button with the given number that is in a list (not used by the library), you can use it.
    void start(int orientation,uint16_t *page_backColors , int backlight, int first_page=0); // Starts TFT screen and draws page 0. Call only once and at the end of setup().
    uint16_t dim(uint16_t color);
    void changeBrightness(byte value);
    void navigatePage( int page , int transition = DEFAULT); // Navigates to a page. This can be used in loop(). It is implicitly called by start() and recalling it causes another draw (only waste of time)
    
        void drawButton(int page,int i); // Draws a button. Don't use publicly. It will draw even if the button is not in current page.
        int addbutton( int page, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, String text,
                   uint16_t textcolor = TFT_WHITE, uint16_t backcolor = TFT_DARKGREY, uint16_t bordercolor = AUTO, bool enabled = true, 
                   bool visible = true,int radius=0,int XtextOffset=0,int YtextOffset=0 )  ; // Adds a button. Use before start()
        void undrawButton(int page,int i); // Hides a button. Don't use publicly
        void changeButtonProperty(int page, int i, int propertyName, int val,int a=0); // Changes a button's property
        void changeButtonProperty(int page, int i, int propertyName, bool val,bool a=false); // Changes a button's property
        void changeButtonProperty(int page, int i, int propertyName, String val, String a = ""); // Changes a button's property
    
    
        int addlabel(int page, uint16_t X, uint16_t Y, String text,
                     uint16_t textcolor, const GFXfont *font = NULL,
                     byte textsizeX = 1, byte textsizeY = 1,
                     bool enabled = true, bool visible = true); // Adds a label. Use before start()
        void drawlabel(int page, int i, bool setTextColor=true); // Draws a label. Don't use publicly
        void undrawlabel(int page, int i); // Hides a label. Don't use publicly
        void changeLabelXPos(int page, int i, uint16_t val); // Changes X position of a label
        void changeLabelYPos(int page, int i, uint16_t val); // Changes Y position of a label
        void changeLabelText(int page, int i, String val); // Changes the text of a label
        void changeLabelTextColor(int page, int i, uint16_t val); // Changes the text color of a label.
        void changeLabelFont(int page, int i, const GFXfont *val); // Changes the text font of a label.
        void changeLabelEnabled(int page, int i, bool val); // Enables or Disables a label.
        void changeLabelVisible(int page, int i, bool val); // Shows or hides a label.
        void changeLabelTextSize(int page, int i, byte X,byte Y); // Changes the text size of a label. If more than 1, it will be blocky. If you don't want it, use a bigger font.
    
        int addslider(uint16_t page,uint16_t X,uint16_t Y, uint16_t W=AUTO,uint16_t H=10,
        uint16_t AreaHeight=AUTO, int value=0,int min=0,int max=AUTO, uint16_t ColorLeft=TFT_BLACK,
        uint16_t colorRight=TFT_BLACK,uint16_t colorBorder=TFT_WHITE,
        uint16_t colorThumb=TFT_WHITE,uint16_t thumbWidth=AUTO,bool enabled=true,bool visible=true);
        void drawSlider(uint16_t page,uint16_t i);
        void undrawSlider(uint16_t page,uint16_t i);
        int getSliderValue(uint16_t page,uint16_t i,int min=0,int max=100);
    
        void drawCheckBox(int page,int i); // Draws the check box
        void undrawCheckBox(int page,int i);
        int  addcheckBox(int page, uint16_t Xpos, uint16_t Ypos, String text, int size=SIZE12PT28PX,bool checked=false, uint16_t checkColor=TFT_BLACK,uint16_t textColor=TFT_WHITE, uint16_t boxColor=TFT_WHITE, uint16_t boxBorder=TFT_BLACK,bool enabled=true,bool visible=true); // Adds a check box. Use before start().
        void changeCheckBoxChecked(int page,int i,bool val); // Checks or unchecks a check-box.
        void changeCheckBoxEnabled(int page,int i,bool val); // Enables or disables a check-box.
        void changeCheckBoxVisible(int page,int i,bool val); // Hides or shows a check-box.
    
    void scrOff(bool fast=false);//Turns off the screen
    void scrOn();//Turns on the screen
    void checkPage( ); // Checks the current page (Updates button, checkbox and slider states)

//Global variables

    int pixel_x//The X position of the touch
     ,pixel_y//The Y position of the touch
     ,pixel_z//The pressure for the touch
     ,CurrentPage=0; //The currently visible page
    float BC=1.0;   //Display brightness
    unsigned long lastTouch; //The millis of the last time screen sensed a touch 
    bool screenOn=true; // Is the display turned on?
    uint16_t *page_backColors;
    
    //Button variables
        uint16_t *button_counts    ;    //1D- Number of buttons in each page

        uint16_t **button_Xpos      ;    //2D- X positions
        uint16_t **button_Ypos      ;    //2D- Y positions
        uint16_t **button_width     ;    //2D- Widths
        uint16_t **button_height    ;    //2D- Heights
        String   **button_text      ;    //2D- Labels
        uint16_t **button_textColor ;    //2D- Text colors
        uint16_t **button_backColor ;    //2D- Back colors
        uint16_t **button_border    ;    //2D- Border colors
        uint16_t **button_XTO       ;    //2D- X text offset
        uint16_t **button_YTO       ;    //2D- Y .... ......
        uint16_t **button_radius    ;    //2D- Border radius
        bool     **button_enabled   ;    //2D- Is button enabled?
        bool     **button_visible   ;    //2D- Is button shown?
        bool     **button_pressed   ;    //2D- Is button pressed?
        
    //Label variables
        uint16_t *label_counts     ;   //1D- Number of labels in each 
    
        uint16_t **label_Xpos       ;   //2D- X positions
        uint16_t **label_Ypos       ;   //2D- Y positions
        String   **label_text       ;   //2D- Texts
        uint16_t **label_textColor  ;   //2D- Text colors
        uint16_t **label_textSizeX  ;   //2D- Text width coefficient
        uint16_t **label_textSizeY  ;   //2D- Text height coefficient
        GFXfont  ***label_font      ;   //2D- Text fonts
        bool     **label_enabled    ;   //2D- Is label enabled?
        bool     **label_visible    ;   //2D- Is label shown?
        
    //Check box variables
        uint16_t *checkbox_counts    ;    //1D- Number of check boxes in each page

        uint16_t **checkbox_Xpos      ;    //2D- X positions
        uint16_t **checkbox_Ypos      ;    //2D- Y positions
        String   **checkbox_text      ;    //2D- Texts beside text boxes
        uint16_t **checkbox_size      ;    //2D- Text and box sizes
        uint16_t **checkbox_checkColor;    //2D- Check mark color
        uint16_t **checkbox_textColor ;    //2D- Text colors
        uint16_t **checkbox_boxColor  ;    //2D- Check box background color
        uint16_t **checkbox_boxBorder ;    //2D- Check box border color
        bool     **checkbox_checked   ;    //2D- Is the check box checked?
        bool     **checkbox_enabled   ;    //2D- Is the check box clickable?
        bool     **checkbox_visible   ;    //2D- Is the check box visible?
        bool     **checkbox_lastState ;    //2D- Private variable, do not use in program code
        
    //Slider variables
        uint16_t *slider_counts            ;    //1D- Number of sliders in each page

        uint16_t **slider_Xpos             ;    //2D- X position
        uint16_t **slider_Ypos             ;    //2D- Y position
        uint16_t **slider_color_border     ;    //2D- Border color
        uint16_t **slider_color_left       ;    //2D- Left color
        uint16_t **slider_color_right      ;    //2D- Right color
        uint16_t **slider_color_thumb      ;    //2D- Thumb color
        uint16_t **slider_height           ;    //2D- Height
        uint16_t **slider_width            ;    //2D- Width
        uint16_t **slider_touch_area_height;    //2D- Area height
        uint16_t **slider_thumb_width      ;    //2D- Thumb width
        uint16_t **slider_value            ;    //2D- Value (NOT MAPPED TO MIN AND MAX!)
        bool     **slider_enabled          ;    //2D- Is the slider changable?
        bool     **slider_visible          ;    //2D- Is the slider visible?
};
#endif // __GUI900_H__