
//Default configs
    #ifndef PAGES
        #define PAGES 10
    #endif
    #ifndef Orientation
        #define Orientation 0
    #endif
    #ifndef MINPRESSURE
        #define MINPRESSURE 50
    #endif
    #ifndef MAXPRESSURE
        #define MAXPRESSURE 1000
    #endif
    #ifndef BACKLIGHT_PIN
        #define BACKLIGHT_PIN -1
    #endif

bool quit; // Used for runsketch()
//Libraries
    #include <Adafruit_GFX.h>                       // Core graphics library, dependency of MCUFRIEND_kbv
    #include <MCUFRIEND_kbv.h>                      // Hardware-specific library
    #include <TouchScreen.h>                        // Touch library
    #include <FreeDefaultFonts.h>                   // Some normal fonts
    #ifndef DISABLEFONTS
        #include <Fonts/FreeSans9pt7b.h>            // Small Sans font
        #include <Fonts/FreeSans12pt7b.h>           // Normal Sans font
        #include <Fonts/FreeSans18pt7b.h>           // Big Sans font
        #include <Fonts/FreeSans24pt7b.h>           // Huge Sans font
    #endif
//Defines
    #define runsketch(stp,lop) stp();while(true){lop();} // Runs an embedded sketch
    #define HCT haschanged=true; // This is the [famous] macro that makes the library aware that something has changed.
    #ifdef BUTTON       // Values used by changeButtonProperty()
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
    #endif
    #define CENTER 9998 // When used, position will be dynamically calculated to align the label to the center
    #define RIGHT 9999  // When used, position will be dynamically calculated to align the label to the right or bottom
    #ifdef CHECKBOX     // Sizes for checkboxes.
        #define SIZE9PT18PX 18  // Use 18 pixels height and FreeSans9pt7b
        #define SIZE12PT28PX 28 // Use 28 pixels height and FreeSans12pt7b
        #define SIZE18PT42PX 42 // Use 42 pixels height and FreeSans18pt7b
        #define SIZE24PT56PX 56 // Use 56 pixels height and FreeSans24pt7b
    #endif
    #define DEFAULT 1   // Default transition for navigation between pages
    #define AUTO 9997   // Used for automatic values. They will be dynamically changed.
//Function prototypes 
    void(* Reset) (void) = 0;   // Soft-resets Arduino microcontroller
    bool Touch_getXY(void);     // Reads touch. If touch is detected, returns true and assigns pixel_x , pixel_y and pixel_z variables to touched coordinates
    int Centre(int Length, int wid = AUTO, int Left = 0, int CHwid = 6); //Calculates the position needed to align an object to the centre
    bool inRegion(int Y, int Bot, int Top = 0, int X = 120, int Left = 0, int Right = AUTO); //If given coordinates are in the given range, returns true.
    int calculateListY(int num); // Calculates the Y position of a button with the given number that is in a list (not used by the library), you can use it.
    void start(); // Starts TFT screen and draws page 0. Call only once and at the end of setup().
    void navigatePage( int page , int transition = DEFAULT); // Navigates to a page. This can be used in loop(). It is implicitly called by start() and recalling it causes another draw (only waste of time)
    #ifdef BUTTON // Button functions
        void drawButton(int page,int i); // Draws a button. Don't use publicly. It will draw even if the button is not in current page.
        int addbutton( int page, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, String text,
                    uint16_t textcolor = TFT_WHITE, uint16_t backcolor = TFT_DARKGREY, uint16_t bordercolor = AUTO, bool enabled = true, 
                    bool visible = true,int radius=0,int XtextOffset=0,int YtextOffset=0 )  ; // Adds a button. Use before start()
        void undrawButton(int page,int i); // Hides a button. Don't use publicly
        void changeButtonProperty(int page, int i, int propertyName, int val,int a=0); // Changes a button's property
        void changeButtonProperty(int page, int i, int propertyName, bool val,bool a=false); // Changes a button's property
        void changeButtonProperty(int page, int i, int propertyName, String val, String a = ""); // Changes a button's propery
    #endif
    #ifdef LABEL  // Label functions
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
        void changeLabelEnabled(int page, int i, bool val); // Dims or Undims a label.
        void changeLabelVisible(int page, int i, bool val); // Shows or hides a label.
        void changeLabelTextSize(int page, int i, byte X,byte Y); // Changes the text size of a label. If more than 1, it will be blocky. If you don't want it, use a bigger font.
    #endif
    #ifdef CHECKBOX // Checkbox functions
        void drawCheckBox(int page,int i); // Draws the check box
        int  addcheckBox(int page, uint16_t Xpos, uint16_t Ypos, String text, int size=SIZE12PT28PX,bool checked=false, uint16_t checkColor=TFT_BLACK,uint16_t textColor=TFT_WHITE, uint16_t boxColor=TFT_WHITE, uint16_t boxBorder=TFT_BLACK,bool enabled=true,bool visible=true); // Adds a check box. Use before start().
        void changeCheckBoxChecked(int page,int i,bool val); // Checks or unchecks a check-box.
        void changeCheckBoxEnabled(int page,int i,bool val); // Enables or disables a check-box.
        void changeCheckBoxVisible(int page,int i,bool val); // Hides or shows a check-box.
    #endif
    #if (defined(SCREENTIMEOUT))||(defined (ALLOWSCREENCONTROL)) // Functions for turning screen on/off. Only if SCREENTIMEOUT or ALLOWSCREENCONTROL is defined
        void scrOff(bool fast=false);//Turns off the screen
        void scrOn();//Turns on the screen
    #endif
    void checkPage( ); // Checks the current page (Updates button, checkbox and slider states)
//Global variables
    MCUFRIEND_kbv tft; //The TFT screen object
    TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  // The touch screen object

    int pixel_x//The X position of the touch
     ,pixel_y//The Y position of the touch
     ,pixel_z//The pressure for the touch
     ,CurrentPage=0; //The currently visible page
    float BC=1.0;   //Display brightness
    unsigned long lastTouch; //The millis of the last time screen sensed a touch 
    bool screenOn=true; // Is the display turned on?
    bool haschanged=false; // Has anything changed since the screen was off?
    bool started=false;
#if (defined(SCREENTIMEOUT))||(defined (ALLOWSCREENCONTROL)) //Screen on/ off controls
// Turns screen off
void scrOff(bool fast){ 
    haschanged=false;
  if(screenOn){ // Does not do anything if screen is off.
    #ifdef SCROFFANITIME // Only if an animation time is defined in Config.ino
    if(!fast){ // Does not do the animation if it is told to
        for(int i=255;i>=0;i--){
            analogWrite(BACKLIGHT_PIN,(brightness*BC/255.0)*i); // Writes the value to backlight pin.
            delay(SCROFFANITIME); // A little delay so that user's eyes can see the animation.
        }
    }
    else
    #endif
      digitalWrite(BACKLIGHT_PIN,LOW); // Turn off directly if animation was disabled
    tft.WriteCmdData(0x0010,0x1692); // Put screen to sleep for less power consumption
    screenOn=false; // Update the value
    BC=0; // set backlight coeffecient to zero
  }
}
// Turns screen on
void scrOn(){
    tft.WriteCmdData(0x0010,0x1690); // Wake TFT driver
    delay(100); // Let the TFT driver turn on
    if((!screenOn)&&haschanged){ // If it nessecary, redraw.
        tft.fillScreen(page_backColors[CurrentPage]); //Clear the screen
        navigatePage(CurrentPage); // Draw the page again
    }
    digitalWrite(BACKLIGHT_PIN,HIGH); // Enable backlight.
    screenOn=true; // Update the variable
    BC=1; // Set backlight coeffecient to 1
    haschanged=false; 
}
#endif
// Calculates Y position for listed buttons
int calculateListY(int num){ 
  return 20 + (num * 50); // This is configured for 20px top margin for list, 40px button height and 10px bottom margin for buttons
}
// Reads touch, maps coordinates, and updates variables. It's the 'driver' for touch screen
bool Touch_getXY(void) { 
    // Most of this function is from Touch_shield_new example.
    TSPoint tp = ts.getPoint();   // Read the touch

    // if sharing pins, you'll need to fix the directions of the touchscreen pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    digitalWrite(XM, HIGH);
    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    bool touch= tp.z > MINPRESSURE && tp.z < MAXPRESSURE;

    // For touch debouncing
    // If last detected touch was at most 100ms ago, it is still being touched.
    unsigned long a=millis(); 
    if(touch)
        lastTouch=a;
    if((!touch)&&((a-lastTouch)<100))
        return true;

    // Now map the coordinates and update the vars
    if (touch) {
        pixel_z=tp.z; // Assign pressure. Only for pages with exit triggered by hard touch
        // Calibration is true for PORTRAIT. tp.y is always long dimension 
        // map to your current pixel orientation
        switch (Orientation) {
            case 0: // Portrait
                pixel_x = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
                pixel_y = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
                break;
            case 1: // Landscape
                pixel_x = map(tp.y, TS_TOP, TS_BOT, 0, tft.width());
                pixel_y = map(tp.x, TS_RT, TS_LEFT, 0, tft.height());
                break;
            case 2: // portrait revrse
                pixel_x = map(tp.x, TS_RT, TS_LEFT, 0, tft.width());
                pixel_y = map(tp.y, TS_BOT, TS_TOP, 0, tft.height());
                break;
            case 3: // Landscape reverse
                pixel_x = map(tp.y, TS_BOT, TS_TOP, 0, tft.width());
                pixel_y = map(tp.x, TS_LEFT, TS_RT, 0, tft.height());
                break;
        }
        return true;
    }
    return false; // Wasnt touhed.
}
#ifndef DONOTDIMDISABLED
    // Gets a color value, and returns a new color value with half R,G,B values
    uint16_t dim(uint16_t color){ 
        int R = ((color & 0xF800) >> 9);  // Get half of Red
        int G = ((color & 0x7E0) >> 4);   // Get half of green
        int B = ((color & 0x1F) << 2);    // Get half of blue
        return tft.color565(R, G, B); //Re-assemble the color
    }
    #else
    #define dim(x) (x)
#endif
// Starts TFT screen and draws page 0. Call only once and at the end of setup().
void start() {
    pinMode(BACKLIGHT_PIN,OUTPUT); // Set the backlight pin as an output
    tft.reset(); // Resets the TFT
    uint16_t ID = tft.readID(); // Different TFT controllers may be used, this identifies the controller model
    tft.begin(ID); // Inits the TFT screen
    if(ID==0x9325) // This only works with ILI9325. for other controllers, you need a different command.
        tft.WriteCmdData(0x2B, 0x000E); // Set frame rate to 112 fps. Used for better look in screen off animation Remember to change this low-level command to match your controller
    delay(100); // Wait a little to rest the shield (may not affect anything)
    tft.setRotation(Orientation); // Sets rotation to predefined value
    tft.fillScreen(page_backColors[CurrentPage]); // Fills the screen with the firt page's color. Filling with black before it only wastes time.
    navigatePage(0); // Draw the first page.
    analogWrite(BACKLIGHT_PIN,brightness*BC); // Activate backlight
    started=true;
}
// Changes the brightness of the TFT screen using backlight control
void changeBrightness(byte value){
    brightness=value; // Update brightness var
    analogWrite(BACKLIGHT_PIN,brightness*BC); // Write to the backlight pin
}
//If given coordinates are in the given range, returns true.
bool inRegion(int Y, int Bot, int Top, int X, int Left, int Right) {
    if(Right==AUTO){
        Right=tft.height();
    }
    if ((
        X >= Left &&
        X <= Right &&
        Y >= Top &&
        Y <= Bot ))
    {
        return true;
    }
  else return false;
}
//Calculates the position needed to align an object to the centre
int Centre(int Length, int wid, int Left, int CHwid) {
  if (wid == AUTO)
    wid = tft.width(); // Expand AUTO macro
  return ((wid / 2) - (Length * CHwid / 2)) + Left; // Half of wid minus Length times half of CHwid plus Left
}
#ifdef BUTTON
    //Button variables
        uint16_t button_counts[PAGES];                      // Number of buttons in each page
        uint16_t button_Xpos      [PAGES][BUTTON];    // X positions
        uint16_t button_Ypos      [PAGES][BUTTON];    // Y posisions
        uint16_t button_width     [PAGES][BUTTON];    // Widths
        uint16_t button_height    [PAGES][BUTTON];    // Heights
        String   button_text      [PAGES][BUTTON];    // Labels
        uint16_t button_textColor [PAGES][BUTTON];    // Text colors
        uint16_t button_backColor [PAGES][BUTTON];    // Back colors
        uint16_t button_border    [PAGES][BUTTON];    // Border colors
        uint16_t button_XTO       [PAGES][BUTTON];    // X text offset
        uint16_t button_YTO       [PAGES][BUTTON];    // Y .... ......
        uint16_t button_radius    [PAGES][BUTTON];    // Border radius
        bool     button_enabled   [PAGES][BUTTON];    // Is button enabled?
        bool     button_visible   [PAGES][BUTTON];    // Is button shown?
        bool     button_pressed   [PAGES][BUTTON];    // Is button pressed?
    int addbutton( int page, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, String text,
                   uint16_t textcolor, uint16_t backcolor, uint16_t bordercolor, bool enabled, 
                   bool visible, int radius, int XtextOffset, int YtextOffset ) 
    {
        HCT // Make the rest of library aware
        if(bordercolor==AUTO){ // Set automatic border color
            if((textcolor==TFT_WHITE) && (backcolor==TFT_DARKGREY)){ //If other colors were given implicitly too,
                bordercolor=TFT_LIGHTGREY; // Use the default border color
            }
            else{
                bordercolor=backcolor; // Use the same border color as background color
            }
        }
        if(radius==CIRCLE){ // Expand CIRCLE macro
            radius=min(Width,Height)/2; // half of the smallest between height and width
        }

        int number = button_counts[page]++; // Current button number

        // Set the variables
        button_Xpos      [page][number] = X;
        button_Ypos      [page][number] = Y;
        button_width     [page][number] = Width;
        button_height    [page][number] = Height;
        button_text      [page][number] = text;
        button_textColor [page][number] = textcolor;
        button_backColor [page][number] = backcolor;
        button_border    [page][number] = bordercolor;
        button_enabled   [page][number] = enabled;
        button_visible   [page][number] = visible;
        button_pressed   [page][number] = false;
        button_XTO       [page][number] = XtextOffset;
        button_YTO       [page][number] = YtextOffset;
        button_radius    [page][number] = radius;

        if(started&&(CurrentPage==page)){
            drawButton(page,number);
        }
        
        return number; // number is the new button's index.
    }
    void drawButton(int page, int i) {
        HCT
        if (button_visible[page][i]) { // Only draw if the button is visible

            // Get values
            uint16_t X           = button_Xpos      [page][i];
            uint16_t Y           = button_Ypos      [page][i];
            uint16_t Width       = button_width     [page][i];
            uint16_t Height      = button_height    [page][i];
            String   text        = button_text      [page][i];
            uint16_t textcolor   = button_textColor [page][i];
            uint16_t backcolor   = button_backColor [page][i];
            uint16_t bordercolor = button_border    [page][i];
            bool     enabled     = button_enabled   [page][i];
            uint16_t radius      = button_radius    [page][i];

            // Calculate center and right aligns
            if(X==CENTER){ // Expand CENTER macro for X position
                X=Centre(Width, tft.width(), 0, 1); // use Centre() here
            }
            if(X==RIGHT){ // Expand RIGHT macro for Y position
                X=tft.width()-Width; // Container width minus object width
            }
            if(Y==CENTER){ // Expand CENTER macro for Y position
                Y=Centre(Height, tft.height(), 0, 1); // Use Centre() here
            }
            if(Y==RIGHT){ // Expand RIGHT macro for Y position
                Y=tft.height()-Height; // Container height minus object height
            }
            if (!enabled) { // dim the button if it is disabled
                textcolor   = dim(textcolor  ); // Dim text color
                backcolor   = dim(backcolor  ); // Dim background color
                bordercolor = dim(bordercolor); // Dim border color
            }
            tft.fillRoundRect(X , Y , Width , Height, radius , backcolor   ); // Draw button background
            if(backcolor!=bordercolor) // To increase speed, draw border only if it has a different color than button background
                tft.drawRoundRect(X , Y , Width , Height ,radius, bordercolor ); // Draw button border
            tft.setTextColor(textcolor); // Sets the foreground color
            int16_t x = 0, y = 0, X1, Y1; // variables needed to use getTextBounds()
            uint16_t W, H;
            #ifndef DISABLEFONTS
            tft.setFont(&FreeSans24pt7b); // Use the biggest font, size will be reduced if needed
            tft.getTextBounds(text, x, y, &X1, &Y1, &W, &H); // Calculate text size
                if ((W > Width) || (H > Height)) { // Does the text exceed the button area?
                    tft.setFont(&FreeSans18pt7b); // Sets a small font
                    tft.getTextBounds(text, x, y, &X1, &Y1, &W, &H); // Checks text size again

                    if ((W > Width) || (H > Height)) {
                        tft.setFont(&FreeSans12pt7b);
                        tft.getTextBounds(text, x, y, &X1, &Y1, &W, &H);

                        if ((W > Width) || (H > Height)) {
                            tft.setFont(&FreeSans9pt7b);
                            tft.getTextBounds(text, x, y, &X1, &Y1, &W, &H);
                        }
                    }
                }
            #else
                tft.setFont(NULL);
                tft.getTextBounds(text, x, y, &X1, &Y1, &W, &H);
            #endif
            int txtX = Centre( W, Width , X, 1); // Calculate the X position where text should be printed
            int txtY = Centre( H, Height, Y, 1); // ......... ... Y ........ ..... .... ...... .. .......
            tft.setTextSize(1, 1); // Assign the text size
            #ifndef DISABLEFONTS
            tft.setCursor(txtX -   2+button_XTO[page][i],
                          txtY + H-2+button_YTO[page][i]); // Assign the text position
            #else
            tft.setCursor(txtX -   2+button_XTO[page][i],
                          txtY +button_YTO[page][i]); // Assign the text position
            #endif
            tft.print(text); // Print the text at last
        }
    }
    void undrawButton(int page, int i) {
        HCT
        int X=button_Xpos[page][i];// Get X position
        int Y=button_Ypos[page][i];// Get Y position
        int Width=button_width[page][i];// Get width
        int Height= button_height[page][i];// Get height
        if(X==CENTER){// Calculate correct position for horizontal center align
            X=Centre(Width, tft.width(), 0, 1);
        }
        if(X==RIGHT){// Calculate correct position for right align
            X=tft.width()-Width;
        }
        if(Y==CENTER){// Calculate correct position for vertical center align
            Y=Centre(Height, tft.height(), 0, 1);
        }
        if(Y==RIGHT){// Calculate correct position for bottom align
            Y=tft.height()-Height;
        }
        tft.fillRect(X, Y, Width, Height, page_backColors[page]); // Fills the area of the button with the page background
    }
    void changeButtonProperty(int page, int i, int propertyName, int val, int a) {
        switch (propertyName) { // Which property must be changed?
            case XPOS: // X position
                if (button_Xpos[page][i] != val) { // Only change the value if it is really changed
                    HCT
                    if (CurrentPage == page) { // Redraw the button only if it's on the current page
                        undrawButton(page, i); // Undraws the current button
                        button_Xpos[page][i] = val; // Assigns the value
                        drawButton(page, i); // Draws the new button
                    }
                    else {
                        button_Xpos[page][i] = val; // Just assign the var if it is invisible
                    }
                }
                break;
            case YPOS: // Y position
              if (button_Ypos[page][i] != val) { // Only change the value if it is really changed
                HCT
                if (CurrentPage == page) { // Redraw the button only if it's on the current page
                  undrawButton(page, i); // Undraws the current button
                  button_Ypos[page][i] = val; // Assigns the value
                  drawButton(page, i); // Draws the new button
                }
                else {
                  button_Ypos[page][i] = val; // Just assign the var if it is invisible
                }
              }
                break;
            case WIDTH: // Width 
                if (button_width[page][i] != val) { // Only change the value if it is really changed
                    HCT
                    if (CurrentPage == page) { // Redraw the button only if it's on the current page
                        if (val < button_width[page][i]) { // Undrawing if the new button became larger is just a waste of time.
                            undrawButton(page, i);
                        }
                        button_width[page][i] = val; // Assign the value
                        drawButton(page, i); // Draw the new button
                    }
                    else {
                        button_width[page][i] = val; // Just assign the var if it is invisible
                    }
                }
                break;
            case HEIGHT: // Height
                if (button_height[page][i] != val) {
                    HCT
                    if (CurrentPage == page) {
                        if (val < button_height[page][i]) {
                            undrawButton(page, i);
                        }
                        button_height[page][i] = val;
                        drawButton(page, i);
                    }
                    else {
                        button_height[page][i] = val;
                    }
                }
                break;
            case TEXT: // Text
                changeButtonProperty(page, i, propertyName, String(val), String("")); // Use already written code
                break;
            case TEXT_COLOR: // Text color
                if (button_textColor[page][i] != val) {
                    HCT
                    button_textColor[page][i] = val;
                    if (CurrentPage == page) {
                        drawButton(page, i);
                    }
                }
                break;
            case BACK_COLOR: // Background color
                if (button_backColor[page][i] != val) {
                    HCT
                    button_backColor[page][i] = val;

                    if (CurrentPage == page) {
                        drawButton(page, i);
                    }
                }
                break;
            case BORDER_COLOR: // Border color
                if (button_border[page][i] != val) {
                    HCT
                    button_border[page][i] = val;
                    if (CurrentPage == page) { // Just draw the border
                        // Override the previous border
                        tft.drawRect(button_Xpos[page][i] , button_Ypos[page][i] , button_width[page][i] , button_height[page][i] , val   );
                    }
                }
                break;
            case VISIBLE:
            case ENABLED: // Visible and enabled
                changeButtonProperty(page, i, propertyName, (val == 1), false); // To reduce program size, we use pre-defined code. This has a very very small impact on performance (only 1 microsecond slower)
                break;
        }
    }
    void changeButtonProperty(int page, int i, int propertyName, bool val, bool a) {
        switch (propertyName) {
            case XPOS:
            case YPOS:
            case WIDTH:
            case HEIGHT: // X position, Y position, Width and height
                changeButtonProperty(page, i, propertyName, ((val) ? 1 : 0), 0); // Use already written code. True:1 False:0
                break;
            case TEXT:
                changeButtonProperty(page, i, propertyName, ((val) ? "True" : "False"), ""); // Use already written code. True:'True' False:'False'
                break;
            case TEXT_COLOR:
            case BACK_COLOR:
            case BORDER_COLOR: // Colors
                changeButtonProperty(page, i, propertyName, ((val) ? TFT_WHITE : TFT_BLACK), 0); // True:White False:Black
                break;
            case ENABLED: // Enabled
                if (button_enabled[page][i] != val) {
                    HCT
                    button_enabled[page][i] = val;
                    if(!val){ // Prevents bugs when this code runs when the button is being touched.
                        button_pressed[page][i] = false;
                    }
                    if (CurrentPage == page) {
                        drawButton(page, i);
                    }
                }
                break;
            case VISIBLE: // Visible
                if ((CurrentPage == page) && (button_visible[page][i] != val)) {
                    HCT
                    button_visible[page][i] = val;
                    if (val == false) {
                        button_pressed[page][i] = false; // Prevents bugs when this code runs when the button is being touched.
                        undrawButton(page, i); 
                    }
                    else {
                        drawButton(page, i);
                    }
                }
                button_visible[page][i] = val;
                break;
        }

    }
    void changeButtonProperty(int page, int i, int propertyName, String val, String a) {
        if(propertyName==TEXT){ // Only Text can be changed here
            if (button_text[page][i] != val) {
                HCT
                button_text[page][i] = val;
                drawButton(page, i);
            }
        }
    }
    void checkButtonTouched(int page, int i){
        if ((button_enabled[page][i])&&(button_visible[page][i])) { // Disabled buttons cannot be clicked
            uint16_t X           = button_Xpos      [page][i]; // X
            uint16_t Y           = button_Ypos      [page][i]; // Y
            uint16_t Width       = button_width     [page][i]; // Get width
            uint16_t Height      = button_height    [page][i]; // Get height
            // Expand macros
            if(X==CENTER){
                X=Centre(Width, tft.width(), 0, 1);
            }
            if(X==RIGHT){
                X=tft.width()-Width;
            }
            if(Y==CENTER){
                Y=Centre(Height, tft.height(), 0, 1);
            }
            if(Y==RIGHT){
                Y=tft.height()-Height;
            }
            // Use inRegion to see if the button is pressed
            button_pressed[page][i] = inRegion(pixel_y, Y + Height, Y, pixel_x, X, X + Width); // Check if it's in the button area
        }
    }
    void checkButtonUntouched(int page, int i){
        button_pressed[page][i] = false;
    }
#endif
#ifdef LABEL
    //Label variables
        uint16_t label_counts[PAGES];                 // Number of labels in each page
        uint16_t label_Xpos       [PAGES][LABEL];     // X positions
        uint16_t label_Ypos       [PAGES][LABEL];     // Y positions
        String   label_text       [PAGES][LABEL];     // Texts
        uint16_t label_textColor  [PAGES][LABEL];     // Text colors
        byte     label_textSizeX  [PAGES][LABEL];     // Text width coeffecient
        byte     label_textSizeY  [PAGES][LABEL];     // Text height coeffecient
        GFXfont  *label_font      [PAGES][LABEL];     // Text fonts
        bool     label_enabled    [PAGES][LABEL];     // Is label enabled?
        bool     label_visible    [PAGES][LABEL];     // Is label shown?
    int addlabel(int page, uint16_t X, uint16_t Y, String text, uint16_t textcolor,
                 const GFXfont *font, byte textsizeX , byte textsizeY ,
                 bool enabled , bool visible = true) 
    {
        HCT
        int number = label_counts[page]++; // Current label number

        // Assign variables
        label_Xpos       [page][number] = X;
        label_Ypos       [page][number] = Y;
        label_text       [page][number] = text;
        label_textColor  [page][number] = textcolor;
        label_font       [page][number] = (GFXfont *)font;
        label_enabled    [page][number] = enabled;
        label_visible    [page][number] = visible;
        label_textSizeX  [page][number] = textsizeX;
        label_textSizeY  [page][number] = textsizeY;

        if (started && (CurrentPage == page)) {
          drawlabel(page, number);
        }

        return number; // Return index of new label
    }
    void drawlabel(int page, int i, bool setTextColor=true) {
        HCT
        if ((label_visible[page][i]) || 
            (!setTextColor)) { // Draw only if the label is visible.

            tft.setFont(label_font[page][i]); // Set the font defined in addlabel()
            tft.setTextSize(label_textSizeX [page][i],
                            label_textSizeY [page][i]); // Set the text size
            if(setTextColor){
                uint16_t color = label_textColor[page][i]; // Get the foreground color to use it later easier (it will be used multiple times)
                if (!label_enabled[page][i]) { // Dim the text if it is disabled
                    color = dim(color);
                }
                tft.setTextColor(color); // Set the color of the text
            }
            int X = label_Xpos[page][i]; // Get the X position for easier use
            int Y = label_Ypos[page][i]; // ... ... Y ........ ... ...... ...
            tft.setTextWrap(X==0);
            if (X == CENTER) { // Calculate the X position if it should be aligned to center
                if (label_font[page][i] == NULL) { // A short-cut for default 8*6 font
                    X = Centre(label_text[page][i].length()); // ( Screen width / 2 ) - ( Text length * 3 ) 
                }
                else { // We need to calculate text size if a costum font is defined
                    int16_t x = 0, y = 0, X1, Y1;  // Variables needed to use getTextBounds()
                    uint16_t W, H;                 // ......... ...... .. ... ...............
                    String a = label_text[page][i];// ......... ...... .. ... ...............
                    tft.getTextBounds(a, x, y, &X1, &Y1, &W, &H); // Calculate the text size
                    X = Centre(W, tft.width(), 0, 1); // Calculate the position needed
                }
            }
            if (X == RIGHT) { // Calculate the X position if it should be aligned to right
                if (label_font[page][i] == NULL) { // A short-cut for default 8*6 font
                    X = tft.width() - (label_text[page][i].length() * 6); // Substract text width from screen width
                }
                else { // Custom font
                    int16_t x = 0, y = 0, X1, Y1; // Vars for getTextBounds
                    uint16_t W, H;
                    String a = label_text[page][i];

                    tft.getTextBounds(a, x, y, &X1, &Y1, &W, &H); // Calculate text size
                    X = tft.width() - W-5; // Container width - text width - 5px
                }
            }
            if (Y == CENTER) { // Expand CENTER macro for Y position
                if (label_font[page][i] == NULL) { // Classic 8x6 font
                    Y = Centre(1, tft.height(), 0, 8); // Use Centre
                }
                else { // Custom font
                    int16_t x = 0, y = 0, X1, Y1; // Values for getTextBounds
                    uint16_t W, H;
                    String a = label_text[page][i];
                    tft.getTextBounds(a, x, y, &X1, &Y1, &W, &H); // Calculate text height
                    Y = Centre(H, tft.height(), 0, 1); // Calculate Y position
                }
            }
            if (Y == RIGHT) { // Expand RIGHT macro for Y positon
                if (label_font[page][i] == NULL) { // Classic 8x6 font
                    Y = tft.height()-8; // 8 pixels higher than the base of screen
                }
                else { //Custom font
                    Y = tft.height() - ((uint8_t)pgm_read_byte(&(label_font[page][i])->yAdvance)); // Screen height - Text height
                }
            }

            if (label_font[page][i] == NULL) { // In default 8*6 font, setCursor() defines the top line
                tft.setCursor(X, Y); // No calculation needed
            }
            else { // In custom fonts, setCursor() defines the bottom line
                int y = (uint8_t)pgm_read_byte(&(label_font[page][i])->yAdvance);  // Get the font height
                y *= label_textSizeY  [page][i]; // Text size coeffecient musn't be forgot
                tft.setCursor(X, Y + y); // Add text height to Y position and assign it
            }
            tft.print(label_text[page][i]); // Print the label at last
        }
    }
    void undrawlabel(int page, int i) { // Uses the code inside drawlabel() but uses the background color of the page
        HCT
        tft.setTextColor(page_backColors[page]);
        drawlabel(page,i,false); // This code tells drawlabel() : "Dont set text color! I want a different color. I've set it before."
    }
    void changeLabelXPos(int page, int i, uint16_t val) { // Changes a label's X position
        if (label_Xpos[page][i] != val) { // Uses the same procedure as changeButtonProperty's code.
            HCT
            if (CurrentPage == page){
                undrawlabel(page, i);
                label_Xpos[page][i] = val;
                drawlabel(page, i);
            }
            else
                label_Xpos[page][i] = val;
        }
    }
    void changeLabelYPos(int page, int i, uint16_t val) { // Changes a label's Y position
        if (label_Ypos[page][i] != val) {
            HCT
            if (CurrentPage == page) {
                undrawlabel(page, i);
                label_Ypos[page][i] = val;
                drawlabel(page, i);
            }
            else
                label_Ypos[page][i] = val;
        }
    }
    void changeLabelText(int page, int i, String val) { // Changes a label's content
        if (label_text[page][i] != val) {
            HCT
            if (CurrentPage == page) {
                undrawlabel(page, i);
                label_text[page][i] = val;
                drawlabel(page, i);
            }
            else
                label_text[page][i] = val;
        }
    }
    void changeLabelTextColor(int page, int i, uint16_t val) { // Changes a label's color
        if (label_textColor[page][i] != val) {
            HCT
            label_textColor[page][i] = val;
            if (CurrentPage == page)
                drawlabel(page, i);
        }
    }
    void changeLabelFont(int page, int i, const GFXfont *val) { // Changes a label's font
        if (label_font[page][i] != val) {
            HCT
            if (CurrentPage == page) {
                undrawlabel(page, i);
                label_font[page][i] = (GFXfont *)val;
                drawlabel(page, i);
            }
        }
        else
            label_font[page][i] = (GFXfont *)val;
    }
    void changeLabelEnabled(int page, int i, bool val) { // Enables or diables a button
        if (label_enabled[page][i] != val) {
            HCT
            label_enabled[page][i] = val;
            if (CurrentPage == page)
                drawlabel(page, i);
        }
    }
    void changeLabelVisible(int page, int i, bool val) { // Hides or shows a label
        if (label_visible[page][i] != val) {
            HCT
            label_visible[page][i] = val;
            if (CurrentPage == page) {
                if (val) {
                    drawlabel(page, i);
                }
                else {
                    undrawlabel(page, i);
                }
            }
        }
    }
    void changeLabelTextSize(int page, int i, byte X, byte Y) { // Changes the zoom of a label
        if ((label_textSizeX[page][i] != X) || (label_textSizeY[page][i] != Y)) {
            HCT
            if (CurrentPage == page) {
                undrawlabel(page, i);
                label_textSizeX[page][i] = X;
                label_textSizeY[page][i] = Y;
                drawlabel(page, i);
            }
            else {
                label_textSizeX[page][i] = X;
                label_textSizeY[page][i] = Y;
            }
        }
    }
#endif
#ifdef CHECKBOX
    //Check box variables
        uint16_t checkbox_counts[PAGES];                  // Number of check boxes in each page
        uint16_t checkbox_Xpos      [PAGES][CHECKBOX];    // X positions
        uint16_t checkbox_Ypos      [PAGES][CHECKBOX];    // Y positions
        String   checkbox_text      [PAGES][CHECKBOX];    // Texts beside text boxes
        int      checkbox_size      [PAGES][CHECKBOX];    // Text and box sizes
        uint16_t checkbox_checkColor[PAGES][CHECKBOX];    // Check mark color
        uint16_t checkbox_textColor [PAGES][CHECKBOX];    // Text colors
        uint16_t checkbox_boxColor  [PAGES][CHECKBOX];    // Check box background color
        uint16_t checkbox_boxBorder [PAGES][CHECKBOX];    // Check box border color
        bool     checkbox_checked   [PAGES][CHECKBOX];    // Is the check box chacked?
        bool     checkbox_enabled   [PAGES][CHECKBOX];    // Is the check box clickable?
        bool     checkbox_visible   [PAGES][CHECKBOX];    // Is the check box visible?
        bool     checkbox_lastState [PAGES][CHECKBOX];    // Private variable, do not use in program code
    int addcheckBox(int page, uint16_t Xpos, uint16_t Ypos, String text, int size=SIZE12PT28PX,
                    bool checked=false, uint16_t checkColor=TFT_BLACK,uint16_t textColor=TFT_WHITE,
                    uint16_t boxColor=TFT_WHITE, uint16_t boxBorder=TFT_BLACK,bool enabled=true,
                    bool visible=true)
    {
        HCT
        int number = checkbox_counts[page]++; // Current label number

        // Assign variables
        checkbox_Xpos       [page][number]=Xpos;
        checkbox_Ypos       [page][number]=Ypos;
        checkbox_text       [page][number]=text;
        checkbox_size       [page][number]=size;
        checkbox_checkColor [page][number]=checkColor;
        checkbox_textColor  [page][number]=textColor;
        checkbox_boxColor   [page][number]=boxColor;
        checkbox_boxBorder  [page][number]=boxBorder;
        checkbox_checked    [page][number]=checked;
        checkbox_enabled    [page][number]=enabled;
        checkbox_visible    [page][number]=visible;
        checkbox_lastState  [page][number]=false;

        if (started && (CurrentPage == page)) {
          drawCheckBox(page, number);
        }

        return number; // Return new check-box's index
    }
    void drawCheckBox(int page,int i){
        HCT
        if((checkbox_visible[page][i])){ // Only draw if it is visible
            // Get values for easier use later
            uint16_t checkColor =checkbox_checkColor [page][i];
            uint16_t textColor  =checkbox_textColor  [page][i];
            uint16_t boxColor   =checkbox_boxColor   [page][i];
            uint16_t borderColor=checkbox_boxBorder  [page][i];

            if(!checkbox_enabled[page][i]){ // Dim if it is disabled
                checkColor =dim(checkColor ); // Dim tick color
                textColor  =dim(textColor  ); // Dim text color
                boxColor   =dim(textColor  ); // Dim box background color
                borderColor=dim(borderColor); // Dim box border color
            }

            // Draw the box

            // Get values for easier use later
            uint16_t X=checkbox_Xpos[page][i];
            uint16_t Y=checkbox_Ypos[page][i];
            uint16_t size=checkbox_size[page][i];

            tft.fillRect(X,Y,size,size,boxColor); // Fill background
            if(boxColor!=borderColor) // Only draw border if it is different
                tft.drawRect(X,Y,size,size,borderColor); // Draw border


            // Draw the check mark
            // Don't try to understand this. Even I (this library's writer) cannot understand it.
            if(checkbox_checked[page][i]){
                int Third=size/3; // Calculate third of size for ease and performance
                tft.drawLine(X+2,Y+(Third*2),X+Third,Y+size-2,checkColor); // Left part
                tft.drawLine(X+Third,Y+size-2,X+size-2,Y+1,checkColor); // Right part
            }

            uint16_t txtY = Y + ( size / 2 ); // Be careful, it's a little confusing

            tft.setTextSize(1, 1); // Reset text size
            // Add to txtY as much as needed depending on box size
            // And set font
            #ifndef DISABLEFONTS
            switch(size){
                case SIZE12PT28PX: // 12pt 28px
                    tft.setFont(&FreeSans12pt7b);
                    txtY+=8;
                    break;
                case SIZE9PT18PX : // 9pt 18px
                    tft.setFont(&FreeSans9pt7b );
                    txtY+=6;
                    break;
                case SIZE18PT42PX: // 18pt 42px
                    tft.setFont(&FreeSans18pt7b);
                    txtY+=12;
                    break;
                case SIZE24PT56PX: // 24pt 56px
                    tft.setFont(&FreeSans24pt7b);
                    txtY+=16;
                    break;
            }
            #else
                tft.setFont(NULL);
                switch (size) {
                    case SIZE12PT28PX: // 12pt 28px
                        tft.setTextSize(3);
                        txtY -= 12;
                        break;
                    case SIZE9PT18PX: // 9pt 18px
                        tft.setTextSize(2);
                        txtY -= 8;
                        break;
                    case SIZE18PT42PX: // 18pt 42px
                        tft.setTextSize(5);
                        txtY -= 20;
                        break;
                    case SIZE24PT56PX: // 24pt 56px
                        tft.setTextSize(7);
                        txtY -= 28;
                        break;
                }
            #endif

            tft.setTextColor(textColor); // Set text color
            tft.setCursor(X+size+5,txtY); // Set text position
            tft.print(checkbox_text[page][i]); // Print the text at last
        }
    }
    void undrawCheckBox(int page,int i){
        HCT
        int size=checkbox_size[page][i]; // Get size for easier use
            #ifndef DISABLEFONTS
            switch(size){
                case SIZE12PT28PX: // 12pt 28px
                    tft.setFont(&FreeSans12pt7b);
                    break;
                case SIZE9PT18PX : // 9pt 18px
                    tft.setFont(&FreeSans9pt7b );
                    break;
                case SIZE18PT42PX: // 18pt 42px
                    tft.setFont(&FreeSans18pt7b);
                    break;
                case SIZE24PT56PX: // 24pt 56px
                    tft.setFont(&FreeSans24pt7b);
                    break;
            }
            #else
                tft.setFont(NULL);
                switch (size) {
                    case SIZE12PT28PX: // 12pt 28px
                        tft.setTextSize(3);
                        break;
                    case SIZE9PT18PX: // 9pt 18px
                        tft.setTextSize(2);
                        break;
                    case SIZE18PT42PX: // 18pt 42px
                        tft.setTextSize(5);
                        break;
                    case SIZE24PT56PX: // 24pt 56px
                        tft.setTextSize(7);
                        break;
                }
            #endif
        tft.setTextSize(1,1); // Reset text size. We are not going to draw any text, we are going to get text size.
        int16_t x = 0, y = 0, X1, Y1; // variables needed to use getTextBounds()
        uint16_t W, H;
        tft.getTextBounds(checkbox_text[page][i], x, y, &X1, &Y1, &W, &H); // Calculate text size
        tft.fillRect(checkbox_Xpos[page][i],checkbox_Ypos[page][i],size+10+W,size,page_backColors[page]); // Fill the check-box with page background color
    }
    void changeCheckBoxXPos(int page, int i, uint16_t val){
        if(checkbox_Xpos[page][i]!=val){
            HCT
            if(CurrentPage==page){
                undrawCheckBox(page,i);
                checkbox_Xpos[page][i]=val;
                drawCheckBox(page,i);
            }
            else{
                checkbox_Xpos[page][i]=val;
            }
        }
    }
    void changeCheckBoxYPos(int page, int i, uint16_t val){
        if(checkbox_Ypos[page][i]!=val){
            HCT
            if(CurrentPage==page){
                undrawCheckBox(page,i);
                checkbox_Ypos[page][i]=val;
                drawCheckBox(page,i);
            }
            else{
                checkbox_Ypos[page][i]=val;
            }
        }
    }
    void changeCheckBoxText(int page, int i, String val){
        if(checkbox_text[page][i]!=val){
            HCT
            if(CurrentPage==page){
                undrawCheckBox(page,i);
                checkbox_text[page][i]=val;
                drawCheckBox(page,i);
            }
            else{
                checkbox_text[page][i]=val;
            }
        }
    }
    void changeCheckBoxSize(int page, int i, uint16_t val){
        if(checkbox_size[page][i]!=val){
            HCT
            if(CurrentPage==page){
                undrawCheckBox(page,i);
                checkbox_size[page][i]=val;
                drawCheckBox(page,i);
            }
            else{
                checkbox_size[page][i]=val;
            }
        }
    }
    void changeCheckBoxCheckColor(int page, int i, uint16_t val){
        if(checkbox_checkColor[page][i]!=val){
            HCT
            checkbox_checkColor[page][i]=val;
            if(CurrentPage==page){
                drawCheckBox(page,i);
            }
        }
    }
    void changeCheckBoxTextColor(int page, int i, uint16_t val){
        if(checkbox_textColor[page][i]!=val){
            HCT
            checkbox_textColor[page][i]=val;
            if(CurrentPage==page){
                drawCheckBox(page,i);
            }
        }
    }
    void changeCheckBoxBoxColor(int page, int i, uint16_t val){
        if(checkbox_boxColor[page][i]!=val){
            HCT
            checkbox_boxColor[page][i]=val;
            if(CurrentPage==page){
                drawCheckBox(page,i);
            }
        }
    }
    void changeCheckBoxBoxBorderColor(int page, int i, uint16_t val){
        if(checkbox_boxBorder[page][i]!=val){
            HCT
            checkbox_boxBorder[page][i]=val;
            if(CurrentPage==page){
                drawCheckBox(page,i);
            }
        }
    }
    void changeCheckBoxChecked(int page,int i,bool val){ // Checks or unchecks a check-box
        if (checkbox_checked[page][i] != val) { // Run the code only if the value has changed
            HCT
            checkbox_checked[page][i] = val; // Assign the value
            if (CurrentPage == page){ // Use embded draw only if it is visible
                bool checked         = val;
                uint16_t X           = checkbox_Xpos      [CurrentPage][i]; // X
                uint16_t Y           = checkbox_Ypos      [CurrentPage][i]; // Y
                uint16_t size        = checkbox_size      [CurrentPage][i]; // Get height
                uint16_t checkColor  = checkbox_checkColor[CurrentPage][i];
                uint16_t boxColor    = checkbox_boxColor  [CurrentPage][i];
                // Draw the check mark
                int Third=size/3;
                tft.drawLine(X+2    ,Y+(Third*2),X+Third ,Y+size-2,(checked)?checkColor:boxColor);
                tft.drawLine(X+Third,Y+size-2   ,X+size-2,Y+1     ,(checked)?checkColor:boxColor);
            }
        }
    }
    void changeCheckBoxEnabled(int page,int i,bool val){ // Enables or disables a check-box
        if (checkbox_enabled[page][i] != val) { // Just like changeLabelEnabled
            HCT
            checkbox_enabled[page][i] = val;
            if (CurrentPage == page){
                drawCheckBox(page,i);
            }
        }
    }
    void changeCheckBoxVisible(int page,int i,bool val){ // Hides or shows a check-box
        if (checkbox_visible[page][i] != val) { // Like changeLabelVisible
            HCT
            checkbox_visible[page][i] = val;
            if (CurrentPage == page){
                if(val){
                    drawCheckBox(page,i);
                }
                else{
                    undrawCheckBox(page,i);
                }
            }
        }
    }
    void checkCheckBoxTouched(int page,int i){
        if((checkbox_enabled[page][i])&&(checkbox_visible[page][i])){
            uint16_t X           = checkbox_Xpos    [page][i];                   // X
            uint16_t Y           = checkbox_Ypos    [page][i];                   // Y
            uint16_t size        = checkbox_size    [page][i];                   // Get height
            // Get width
            int16_t x = 0, y = 0, X1, Y1;                                               // variables needed to use getTextBounds()
            uint16_t W, H;
            tft.getTextBounds(checkbox_text[page][i], x, y, &X1, &Y1, &W, &H);   // Calculate text size
            uint16_t Width       = size+10+W;              // Get width
            // Check if it was pressed
            bool pressed=inRegion(pixel_y, Y + size , Y, pixel_x, X, X + Width);
            if(pressed){ // Toggle states
                if(!checkbox_lastState[page][i]){ // Only if it wasn't being touched before
                    checkbox_checked[page][i]=!checkbox_checked[page][i]; // Toggle value
                    bool checked=checkbox_checked[page][i]; // Get values for easy use
                    uint16_t checkColor=checkbox_checkColor[page][i];
                    uint16_t boxColor  =checkbox_boxColor  [page][i];
                    // Draw the check mark
                    int Third=size/3;
                    tft.drawLine(X+2    ,Y+(Third*2),X+Third ,Y+size-2,(checked)?checkColor:boxColor);
                    tft.drawLine(X+Third,Y+size-2   ,X+size-2,Y+1     ,(checked)?checkColor:boxColor);
                }
            }
            checkbox_lastState[page][i]=true;
        }
    }
    void checkCheckBoxUntouched(int page, int i){
        checkbox_lastState[page][i] = false;
    }
#endif
#ifdef SLIDER
    //Slider variables
        uint16_t slider_counts[PAGES];                  // Number of sliders in each page
        uint16_t slider_Xpos                [PAGES][SLIDER];    // X position
        uint16_t slider_Ypos                [PAGES][SLIDER];    // Y position
        uint16_t slider_color_border        [PAGES][SLIDER];    // Border color
        uint16_t slider_color_left          [PAGES][SLIDER];    // Left color
        uint16_t slider_color_right         [PAGES][SLIDER];    // Right color
        uint16_t slider_color_thumb         [PAGES][SLIDER];    // Thumb color
        uint16_t slider_height              [PAGES][SLIDER];    // Height
        uint16_t slider_width               [PAGES][SLIDER];    // Width
        uint16_t slider_touch_area_height   [PAGES][SLIDER];    // Area height
        uint16_t slider_thumb_width         [PAGES][SLIDER];    // Thumb width
        int      slider_value               [PAGES][SLIDER];    // Value (NOT MAPPED TO MIN AND MAX!)
        int      slider_min                 [PAGES][SLIDER];    // The value when slider is at the leftmost position
        int      slider_max                 [PAGES][SLIDER];    // The value when slider is at the rightmost position
        bool     slider_enabled             [PAGES][SLIDER];    // Is the slider changable?
        bool     slider_visible             [PAGES][SLIDER];    // Is the slider visible?
       
    int addslider(uint16_t page,uint16_t X,uint16_t Y, uint16_t W=AUTO,uint16_t H=10,
        uint16_t AreaHeight=AUTO, int value=0,int min=0,int max=AUTO, uint16_t ColorLeft=TFT_BLACK,
        uint16_t colorRight=TFT_BLACK,uint16_t colorBorder=TFT_WHITE,
        uint16_t colorThumb=TFT_WHITE,uint16_t thumbWidth=AUTO,bool enabled=true,bool visible=true)
    {
        HCT
        int number = slider_counts[page]++; // Current label number
        if(AreaHeight==AUTO){ // Expand AUTO macro for area height
            AreaHeight=H*3; // Three times height
        }
        if(thumbWidth==AUTO){ // Expand AUTO macro
            thumbWidth=min(H,W)*0.7; // 70% of height. For 10px height, it is 7px.
        }
        if(W==AUTO){ // Expand AUTO macro for width
            W=tft.width()-(X*2); // Container width minus two times X position
        }
        if(max==AUTO){ // Expand AUTO macro for maximum
            max=W-thumbWidth; // Width minus thimb width
        }

        // Assign variables
        slider_Xpos                 [page][number]=X;
        slider_Ypos                 [page][number]=Y;
        slider_width                [page][number]=W;
        slider_height               [page][number]=H;
        slider_touch_area_height    [page][number]=AreaHeight;
        slider_value                [page][number]=map(value,min,max,0,W-thumbWidth);
        slider_color_left           [page][number]=ColorLeft;
        slider_color_right          [page][number]=colorRight;
        slider_color_border         [page][number]=colorBorder;
        slider_color_thumb          [page][number]=colorThumb;
        slider_thumb_width          [page][number]=thumbWidth;
        slider_enabled              [page][number]=enabled;
        slider_visible              [page][number]=visible;
        slider_min                  [page][number]=min;
        slider_max                  [page][number]=max;

        if (started && (CurrentPage == page)) {
          drawSlider(page, number);
        }

        return number; // Return new slider's index
    }
    void drawSlider(uint16_t page,uint16_t i){
        // The simplest draw function is here
        HCT
        if(slider_visible[page][i]){ // Only draw if it's visible
            // Get values for easier use
            uint16_t s_X            =slider_Xpos        [page][i];
            uint16_t s_value        =slider_value       [page][i];
            uint16_t s_thumb_width  =slider_thumb_width [page][i];
            uint16_t s_width        =slider_width       [page][i];
            uint16_t s_height       =slider_height      [page][i];
            uint16_t s_color_left   =slider_color_left  [page][i];
            uint16_t s_color_right  =slider_color_right [page][i];
            uint16_t s_color_border =slider_color_border[page][i];
            uint16_t s_color_thumb  =slider_color_thumb [page][i];
            uint16_t s_Y            =slider_Ypos        [page][i];

            if(!slider_enabled[page][i]){ // Dim if slider is disabled
                s_color_border =dim(s_color_border ); // Border color
                s_color_left   =dim(s_color_left   ); // Left color
                s_color_right  =dim(s_color_right  ); // Right color
                s_color_thumb  =dim(s_color_thumb  ); // Thumb color
            }
            
            // Draw the slider
            if(s_height>s_width){
                tft.fillRect(s_X+1,s_Y+1,s_width-2,s_value-1,s_color_left); // Left area
                tft.fillRect(s_X+1,s_Y+s_value,s_width-2,s_thumb_width,s_color_thumb); // Thumb
                tft.fillRect(s_X+1,s_Y+s_value+s_thumb_width,s_width-2,s_height-s_value-s_thumb_width-1,s_color_right);
                tft.drawRect(s_X,s_Y,s_width,s_height,s_color_border); // Border
            }
            else{
                tft.fillRect(s_X+1,s_Y+1,s_value-1,s_height-2,s_color_left); // Left area
                tft.fillRect(s_X+s_value,s_Y+1,s_thumb_width,s_height-2,s_color_thumb); // Thumb
                tft.fillRect(s_X+s_value+s_thumb_width,s_Y+1,s_width-s_value-s_thumb_width-1,s_height-2,s_color_right); // Right area
                tft.drawRect(s_X,s_Y,s_width,s_height,s_color_border); // Border
            }
        }
    }
    void undrawSlider(uint16_t page,uint16_t i){
        HCT
        // Get values for easier use
        uint16_t s_X            =slider_Xpos        [page][i];
        uint16_t s_width        =slider_width       [page][i];
        uint16_t s_height       =slider_height      [page][i];
        uint16_t s_Y            =slider_Ypos        [page][i];

        tft.fillRect(slider_Xpos[page][i],slider_Ypos[page][i],slider_width[page][i],slider_height[page][i],page_backColors[page]); // 'Fill' the border
    }
    int getSliderValue(uint16_t page,uint16_t i,int min,int max){
        //// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED ////
        return map(slider_value[page][i],                               // Input value
                   0,                                                   // Input minimum
                   slider_width[page][i] - slider_thumb_width[page][i], // Input maximum
                   min,                                                 // Output minimum
                   max);                                                // Output maximum
    }
    int getSliderValue(uint16_t page,uint16_t i){
        return map(slider_value[page][i],                               // Input value
                   0,                                                   // Input minimum
                   slider_width[page][i] - slider_thumb_width[page][i], // Input maximum
                   slider_min[page][i],                                 // Output minimum
                   slider_max[page][i]);                                // Output maximum
    }
    void changeSliderXPos(int page,int i,uint16_t val){
        if(slider_Xpos[page][i]!=val){
            HCT
            if (CurrentPage == page){
                undrawSlider(page, i);
                slider_Xpos[page][i] = val;
                drawSlider(page, i);
            }
            else
                slider_Xpos[page][i] = val;
        }
    }
    void changeSliderYPos(int page,int i,uint16_t val){
        if(slider_Ypos[page][i]!=val){
            HCT
            if (CurrentPage == page){
                undrawSlider(page, i);
                slider_Ypos[page][i] = val;
                drawSlider(page, i);
            }
            else
                slider_Ypos[page][i] = val;
        }
    }
    void changeSliderWidth(int page,int i,uint16_t val){
        if (slider_width[page][i] != val) { // Only change the value if it is really changed
            HCT
            int value=getSliderValue(page,i);
            if (CurrentPage == page) { // Redraw the slider only if it's on the current page
                if (val < slider_width[page][i]) { // Undrawing if the new slider became larger is just a waste of time.
                    undrawSlider(page, i);
                }
                slider_width[page][i] = val; // Assign the value
                slider_value[page][i] =
                    map(value, slider_min[page][i], slider_max[page][i], 0,
                        slider_width[page][i] - slider_thumb_width[page][i]);
                drawSlider(page, i); // Draw the new slider
            }
            else {
              slider_width[page][i] =
                  val; // Just assign the var if it is invisible
              slider_value[page][i] =
                  map(value, slider_min[page][i], slider_max[page][i], 0,
                      slider_width[page][i] - slider_thumb_width[page][i]);
            }
        }
    }
    void changeSliderHeight(int page,int i,uint16_t val){
        if (slider_height[page][i] != val) { // Only change the value if it is really changed
            HCT
            if (CurrentPage == page) { // Redraw the slider only if it's on the current page
                if (val < slider_height[page][i]) { // Undrawing if the new slider became larger is just a waste of time.
                    undrawSlider(page, i);
                }
                slider_height[page][i] = val; // Assign the value
                drawSlider(page, i); // Draw the new slider
            }
            else {
                slider_height[page][i] = val; // Just assign the var if it is invisible
            }
        }
    }
    void changeSliderAreaHeight(int page, int i, uint16_t val){
        slider_touch_area_height[page][i]=val;  // Checking if it is changed isn't effecient here. Checking takes almost the same time as updating value.
        // As it isn't about the appearance, it doesn't need redraw.
    }
    int  changeSliderValue(int page, int i, int val, bool changeOther=false){
        // You may have a few questions here.
        // Q: Why does it return int?
        //  A: Because the value is constrained to min&max. It returns the constrained value.
        // Q: Why is value type int and not uint16_t?
        //  A: 1. It must support negative numbers 2. Arduino Due can do 32-bit operations in one instruction, and it is better to use 32-bit. and int is 32-bit in arduino Due.
        // Q: What is changeOther?
        //  A: If it is true and given value is out of range, the range will be resized to fit into it. if it is false, the value will be constrained.
        int min=slider_min[page][i];
        int max=slider_max[page][i];
        int value = map(slider_value[page][i], // Input value
                        0,                     // Input minimum
                        slider_width[page][i] -
                            slider_thumb_width[page][i], // Input maximum
                        min,  // Output minimum
                        max); // Output maximum
        if(value!=val){// It is changed
            if(changeOther){
                if(min>val){
                    slider_min[page][i]=val;
                }
                if(max<val){
                    slider_max[page][i]=val;
                }
            }
            else{
                val=constrain(val,min,max);
            }
            int unMapped = map( val,
                                min,
                                max,
                                0,
                                slider_width[page][i] -
                                    slider_thumb_width[page][i]);
            slider_value[page][i]=unMapped;
            if(CurrentPage==page)
                drawSlider(page,i);
        }
        return val;
    }
    int  changeSliderMinimum(int page,int i, int val, bool changeOther=false){
        int min=slider_min[page][i];
        if(min!=val){
            int max=slider_max[page][i];
            int value = map(slider_value[page][i], // Input value
                            0,                     // Input minimum
                            slider_width[page][i] -
                                slider_thumb_width[page][i], // Input maximum
                            min,  // Output minimum
                            max); // Output maximum
            if(changeOther){
                if(min>value){
                    value=min;
                    int unMapped = map( value,
                                        min,
                                        max,
                                        0,
                                        slider_width[page][i] -
                                            slider_thumb_width[page][i]);
                    slider_value[page][i]=unMapped;
                }
            }
            else{
                min=min(value,min);
            }
            slider_min[page][i]=min;
            if(CurrentPage==page)
                drawSlider(page,i);
        }
    }
    int  changeSliderMaximum(int page,int i, int val, bool changeOther=false){
        int max=slider_max[page][i];
        if(max!=val){
            int min=slider_min[page][i];
            int value = map(slider_value[page][i], // Input value
                            0,                     // Input minimum
                            slider_width[page][i] -
                                slider_thumb_width[page][i], // Input maximum
                            min,  // Output minimum
                            max); // Output maximum
            if(changeOther){
                if(max<value){
                    value=max;
                    int unMapped = map( value,
                                        min,
                                        max,
                                        0,
                                        slider_width[page][i] -
                                            slider_thumb_width[page][i]);
                    slider_value[page][i]=unMapped;
                }
            }
            else{
                max=max(value,max);
            }
            slider_max[page][i]=max;
            if(CurrentPage==page)
                drawSlider(page,i);
        }
    }
    void changeSliderLeftColor(int page,int i,uint16_t val){
        if(slider_color_left[page][i]!=val){
            HCT
            slider_color_left[page][i] = val;
            if (CurrentPage == page)
                drawSlider(page, i);
        }
    }
    void changeSliderRightColor(int page,int i,uint16_t val){
        if(slider_color_right[page][i]!=val){
            HCT
            slider_color_right[page][i] = val;
            if (CurrentPage == page)
                drawSlider(page, i);
        }
    }
    void changeSliderThumbColor(int page,int i,uint16_t val){
        if(slider_color_thumb[page][i]!=val){
            HCT
            slider_color_thumb[page][i] = val;
            if (CurrentPage == page)
                drawSlider(page, i);
        }
    }
    void changeSliderBorderColor(int page,int i,uint16_t val){
        if(slider_color_border[page][i]!=val){
            HCT
            slider_color_border[page][i] = val;
            if (CurrentPage == page)
                drawSlider(page, i);
        }
    }
    void changeSliderBackgroundColor(int page,int i,uint16_t val){
        if((slider_color_left[page][i]!=val)||(slider_color_right[page][i]!=val)){
            HCT
            slider_color_left[page][i] = val;
            slider_color_right[page][i] = val;
            if (CurrentPage == page)
                drawSlider(page, i);
        }
    }
    void changeSliderForegroundColor(int page,int i,uint16_t val){
        if((slider_color_thumb[page][i]!=val)||(slider_color_border[page][i]!=val)){
            HCT
            slider_color_thumb[page][i] = val;
            slider_color_border[page][i] = val;
            if (CurrentPage == page)
                drawSlider(page, i);
        }
    }
    void changeSliderThumbWidth(int page,int i, uint16_t val){
        if(slider_thumb_width[page][i]!=val){
            if(val>slider_width[page][i]){
                return;
            }
            slider_thumb_width[page][i]=val;
            if(CurrentPage==page)
                drawSlider(page,i);
        }
    }
    void changeSliderEnabled(int page,int i, bool val){
        if(slider_enabled[page][i]!=val){
            slider_enabled[page][i]=val;
            if(CurrentPage==page){
                drawSlider(page,i);
            }
        }
    }
    void changeSliderVisible(int page,int i, bool val){
        if(slider_visible[page][i]!=val){
            slider_visible[page][i]=val;
            if(CurrentPage==page){
                if(val)
                    drawSlider(page,i);
                else
                    undrawSlider(page,i);
            }
        }
    }
    void checkSliderTouched(int page, int i){
        if ((slider_enabled[page][i])&&(slider_visible[page][i])) { // Disabled sliders cannot be changed

            uint16_t s_X                = slider_Xpos               [page][i]; // X
            uint16_t s_Y                = slider_Ypos               [page][i]; // Y
            uint16_t s_width            = slider_width              [page][i]; // Get width
            uint16_t s_height           = slider_height             [page][i]; // Get height
            uint16_t s_touch_area_height= slider_touch_area_height  [page][i]; // Area height
            uint16_t s_thumb_width      = slider_thumb_width        [page][i]; // Thumb width
            if(s_width>s_height){
                // Calculate touch region
                int tac=s_Y+(s_height/2);      // Touch area center
                int tah=s_touch_area_height/2; // Half of touch area height
                int tat=tac-tah;               // Touch area top
                int tab=tac+tah;               // Touch area botton
                // Was the slider pressed?
                if(inRegion(pixel_y, tab, tat, pixel_x, s_X, s_X+s_width)){
                    int a=min(pixel_x,(s_X+s_width-s_thumb_width))
                    slider_value[page][i]=pixel_x-s_X; // Update slider value
                    drawSlider(page,i); // Draw slider with new value
                }
            }
            else{
                int tac=s_X+(s_width/2);
                int taw=s_touch_area_height/2;
                int tal=tac-taw;
                int tar=tac+taw;
                if(inRegion(pixel_y,s_Y+s_height,s_Y,pixel_x,tal,tar)){
                    slider_value[page][i]=pixel_y-s_Y; // Update slider value
                    drawSlider(page,i); // Draw slider with new value
                }
            }
        }
    }
#endif
void navigatePage( int page , int transition){ // Navigates to another page
    HCT
    switch (transition) { // Currently only one transition is supported
        case DEFAULT: // Default transition
            if(!((CurrentPage==0)&&(page==0))){ // Do not undraw anything if it was called from start()
                if (page_backColors[CurrentPage] == page_backColors[page]) { // If background colors were the same, only undraw elements, not the whole screen
                    #ifdef BUTTON // Undraw all buttons
                        for (int i = 0; i < button_counts[CurrentPage]; i++) {
                            undrawButton(CurrentPage, i);
                        }
                    #endif
                    #ifdef LABEL // Undraw all labels
                        for (int i = 0; i < label_counts[CurrentPage]; i++) {
                            undrawlabel(CurrentPage, i);
                        }
                    #endif
                    #ifdef CHECKBOX // Undraw all check-boxes
                        for (int i = 0; i < checkbox_counts[CurrentPage]; i++) {
                            undrawCheckBox(CurrentPage, i);
                        }
                    #endif
                    #ifdef SLIDER // Undraw all sliders
                        for (int i = 0; i < slider_counts[CurrentPage]; i++) {
                            undrawSlider(CurrentPage, i);
                        }
                    #endif
                }
                else
                    tft.fillScreen(page_backColors[page]); // if background colors were different, fill the whole screen
            }
            #ifdef BUTTON // Draw all buttons
                for (int i = 0; i < button_counts[page]; i++) {
                    drawButton(page, i);
                }
            #endif
            #ifdef LABEL // Draw all labels
                for (int i = 0; i < label_counts[page]; i++) {
                    drawlabel(page, i);
                }
            #endif
            #ifdef CHECKBOX // Draw all check-boxes
                for (int i = 0; i < checkbox_counts[page]; i++) {
                    drawCheckBox(page, i);
                }
            #endif
            #ifdef SLIDER // Draw all sliders
                for (int i = 0; i < slider_counts[page]; i++) {
                    drawSlider(page, i);
                }
            #endif
            CurrentPage = page; // Set CurrentPage
            break;
    }
}
void checkPage( ) { // Check for touches
    if ( Touch_getXY()) { // Check elements only if somewhere is touched
        #ifdef SCREENTIMEOUT
            // Code to wake screen up
            if(!screenOn){ // Just if screen was off
                scrOn(); // Turn on screen
                while(Touch_getXY()); // Wait until there is no touch
                return; // Do not handle elements, so return
            }
        #endif
        #ifdef BUTTON // Handle buttons
            for (int i = 0; i < button_counts[CurrentPage]; i++) { // Iterate through buttons
                checkButtonTouched(CurrentPage,i);
            }
        #endif
        // Labels are only shown, they do not handle touch.
        #ifdef CHECKBOX // Handle check-boxes
            for(int i=0;i<checkbox_counts[CurrentPage];i++){ // Iterate through check-boxes
                checkCheckBoxTouched(CurrentPage,i);
            }
        #endif
        #ifdef SLIDER // Handle sliders
            for (int i = 0; i < slider_counts[CurrentPage]; i++) { // Iterate through sliders
            checkSliderTouched(CurrentPage,i);
            }
        #endif
    }
  else { // It wasn't pressed
    #ifdef BUTTON
        // Set all pressed states to false
        for (int k = 0; k < PAGES; k++) {
            for (int j = 0; j < button_counts[k]; j++) {
                checkButtonUntouched(k,j);
            }
        }
    #endif
    #ifdef CHECKBOX
        // Set all pressed states to false
        for (int k = 0; k < PAGES; k++) {
            for (int j = 0; j < checkbox_counts[k]; j++) {
                checkCheckBoxUntouched(k,j);
            }
        }
    #endif


    #ifdef SCREENTIMEOUT
        // Did the last touch occur in at least SCREENTIMEOUT seconds ago?
        if(-(lastTouch-millis())>(SCREENTIMEOUT*1000)){
            scrOff(); // Turn off screen
        }
    #endif
  }
}
