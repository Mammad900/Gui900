/*
  Gui900.h - Library for creating an interface for applications using a TFT screen.
  Created by Mohammad Hossein Abedi, July 29, 2020.
  Released into the public domain.
*/
#include "Arduino.h"
#include "Gui900.h"


// Turns screen off
void Gui900::scrOff(bool fast){ 
    haschanged=false;
  if(screenOn){ // Does not do anything if screen is off.
    if(!fast){ // Does not do the animation if it is told to
        for(int i=255;i>=0;i--){
            analogWrite(_backlightPin,(brightness*BC/255.0)*i); // Writes the value to backlight pin.
            delay(2); // A little delay so that user's eyes can see the animation.
        }
    }
    else
      digitalWrite(_backlightPin,LOW); // Turn off directly if animation was disabled
    tft.WriteCmdData(0x0010,0x1692); // Put screen to sleep for less power consumption
    screenOn=false; // Update the value
    BC=0; // set backlight coefficient to zero
  }
}
// Turns screen on
void Gui900::scrOn(){
    tft.WriteCmdData(0x0010,0x1690); // Wake TFT driver
    delay(100); // Let the TFT driver turn on
    if((!screenOn)&&haschanged){ // If it necessary, redraw.
        tft.fillScreen(page_backColors[CurrentPage]); //Clear the screen
        navigatePage(CurrentPage); // Draw the page again
    }
    digitalWrite(_backlightPin,HIGH); // Enable backlight.
    screenOn=true; // Update the variable
    BC=1; // Set backlight coefficient to 1
    haschanged=false; 
}
// Calculates Y position for listed buttons
int Gui900::calculateListY(int num){ 
    return 20 + (num * 50); // This is configured for 20px top margin for list, 40px button height and 10px bottom margin for buttons
}
// Reads touch, maps coordinates, and updates variables. It's the 'driver' for touch screen
bool Gui900::Touch_getXY(void) { 
    // Most of this function is from Touch_shield_new example.
    TSPoint tp = ts.getPoint();   // Read the touch

    // if sharing pins, you'll need to fix the directions of the touchscreen pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    digitalWrite(XM, HIGH);
    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    bool touch= tp.z > minPressure && tp.z < maxPressure;

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
        switch (orientation) {
            case 0: // Portrait
                pixel_x = map(tp.x, tsLeft, tsRight, 0, tft.width());
                pixel_y = map(tp.y, tsTop, tsBottom, 0, tft.height());
                break;
            case 1: // Landscape
                pixel_x = map(tp.y, tsTop, tsBottom, 0, tft.width());
                pixel_y = map(tp.x, tsRight, tsLeft, 0, tft.height());
                break;
            case 2: // portrait revrse
                pixel_x = map(tp.x, tsRight, tsLeft, 0, tft.width());
                pixel_y = map(tp.y, tsBottom, tsTop, 0, tft.height());
                break;
            case 3: // Landscape reverse
                pixel_x = map(tp.y, tsBottom, tsTop, 0, tft.width());
                pixel_y = map(tp.x, tsLeft, tsRight, 0, tft.height());
                break;
        }
        return true;
    }
    return false; // wasn't touched.
}
// Gets a color value, and returns a new color value with half R,G,B values
uint16_t Gui900::dim(uint16_t color){ 
    int R = ((color & 0xF800) >> 9);  // Get half of Red
    int G = ((color & 0x7E0) >> 4);   // Get half of green
    int B = ((color & 0x1F) << 2);    // Get half of blue
    return tft.color565(R, G, B); //Re-assemble the color
}
// Starts TFT screen and draws page 0. Call only once and at the end of setup().
void Gui900::start(int orientation,uint16_t *page_backColors , int backlight, int first_page=0) {
    pinMode(backlight,OUTPUT); // Set the backlight pin as an output
    tft.reset(); // Resets the TFT
    uint16_t ID = tft.readID(); // Different TFT controllers may be used, this identifies the controller model
    tft.begin(ID); // Inits the TFT screen
    if(ID==0x9325) // This only works with ILI9325. for other controllers, you need a different command.
        tft.WriteCmdData(0x2B, 0x000E); // Set frame rate to 112 fps. Used for better look in screen off animation Remember to change this low-level command to match your controller
    delay(100); // Wait a little to rest the shield (may not affect anything)
    tft.setRotation(orientation); // Sets rotation to predefined value
    tft.fillScreen(page_backColors[first_page]); // Fills the screen with the first page's color. Filling with black before it only wastes time.
    navigatePage(0); // Draw the first page.
    analogWrite(backlight,brightness*BC); // Activate backlight
    _backlightPin=backlight;
    this->orientation=orientation;
    this->page_backColors=page_backColors;
}
// Changes the brightness of the TFT screen using backlight control
void Gui900::changeBrightness(byte value){
    brightness=value; // Update brightness var
    analogWrite(_backlightPin,brightness*BC); // Write to the backlight pin
}
//If given coordinates are in the given range, returns true.
bool Gui900::inRegion(int Y, int Bot, int Top, int X, int Left, int Right) {
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
int Gui900::Centre(int Length, int wid, int Left, int CHwid) {
  if (wid == AUTO)
    wid = tft.width(); // Expand AUTO macro
  return ((wid / 2) - (Length * CHwid / 2)) + Left; // Half of wid minus Length times half of CHwid plus Left
}

    int Gui900::addbutton( int page, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, String text,
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
        
        return number; // number is the new button's index.
    }
    void Gui900::drawButton(int page, int i) {
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
            tft.setFont(&FreeSans24pt7b); // Use the biggest font, size will be reduced if needed
            tft.setTextColor(textcolor); // Sets the foreground color
            int16_t x = 0, y = 0, X1, Y1; // variables needed to use getTextBounds()
            uint16_t W, H;
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
            int txtX = Centre( W, Width , X, 1); // Calculate the X position where text should be printed
            int txtY = Centre( H, Height, Y, 1); // ......... ... Y ........ ..... .... ...... .. .......
            tft.setTextSize(1, 1); // Assign the text size
            tft.setCursor(txtX -   2+button_XTO[page][i],
                          txtY + H-2+button_YTO[page][i]); // Assign the text position
            tft.print(text); // Print the text at last
        }
    }
    void Gui900::undrawButton(int page, int i) {
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
    void Gui900::changeButtonProperty(int page, int i, int propertyName, int val, int a) {
        switch (propertyName) { // Which property must be changed?
            case XPOS: // X position
                if (button_Xpos[page][i] != val) { // Only change the value if it is really changed
                    HCT
                    if (CurrentPage == page) { // Redraw the button only if it's on the current page
                        undrawButton(page, i); // Un-draws the current button
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
                  undrawButton(page, i); // Un-draws the current button
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
    void Gui900::changeButtonProperty(int page, int i, int propertyName, bool val, bool a) {
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
    void Gui900::changeButtonProperty(int page, int i, int propertyName, String val, String a) {
        if(propertyName==TEXT){ // Only Text can be changed here
            if (button_text[page][i] != val) {
                HCT
                button_text[page][i] = val;
                drawButton(page, i);
            }
        }
    }

    int Gui900::addlabel(int page, uint16_t X, uint16_t Y, String text, uint16_t textcolor,
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

        return number; // Return index of new label
    }
    void Gui900::drawlabel(int page, int i, bool setTextColor=true) {
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
            if (Y == RIGHT) { // Expand RIGHT macro for Y position
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
                y *= label_textSizeY  [page][i]; // Text size coefficient mustn't be forgot
                tft.setCursor(X, Y + y); // Add text height to Y position and assign it
            }
            tft.print(label_text[page][i]); // Print the label at last
        }
    }
    void Gui900::undrawlabel(int page, int i) { // Uses the code inside drawlabel() but uses the background color of the page
        HCT
        tft.setTextColor(page_backColors[page]);
        drawlabel(page,i,false); // This code tells drawlabel() : "Dont set text color! I want a different color. I've set it before."
    }
    void Gui900::changeLabelXPos(int page, int i, uint16_t val) { // Changes a label's X position
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
    void Gui900::changeLabelYPos(int page, int i, uint16_t val) { // Changes a label's Y position
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
    void Gui900::changeLabelText(int page, int i, String val) { // Changes a label's content
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
    void Gui900::changeLabelTextColor(int page, int i, uint16_t val) { // Changes a label's color
        if (label_textColor[page][i] != val) {
            HCT
            label_textColor[page][i] = val;
            if (CurrentPage == page)
                drawlabel(page, i);
        }
    }
    void Gui900::changeLabelFont(int page, int i, const GFXfont *val) { // Changes a label's font
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
    void Gui900::changeLabelEnabled(int page, int i, bool val) { // Enables or disables a button
        if (label_enabled[page][i] != val) {
            HCT
            label_enabled[page][i] = val;
            if (CurrentPage == page)
                drawlabel(page, i);
        }
    }
    void Gui900::changeLabelVisible(int page, int i, bool val) { // Hides or shows a label
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
    void Gui900::changeLabelTextSize(int page, int i, byte X, byte Y) { // Changes the zoom of a label
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
 
    int Gui900::addcheckBox(int page, uint16_t Xpos, uint16_t Ypos, String text, int size=SIZE12PT28PX,
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

        return number; // Return new check-box's index
    }
    void Gui900::drawCheckBox(int page,int i){
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

            // Add to txtY as much as needed depending on box size
            // And set font
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

            tft.setTextColor(textColor); // Set text color
            tft.setTextSize(1,1); // Reset text size
            tft.setCursor(X+size+5,txtY); // Set text position
            tft.print(checkbox_text[page][i]); // Print the text at last
        }
    }
    void Gui900::undrawCheckBox(int page,int i){
        HCT
        int size=checkbox_size[page][i]; // Get size for easier use
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
        tft.setTextSize(1,1); // Reset text size. We are not going to draw any text, we are going to get text size.
        int16_t x = 0, y = 0, X1, Y1; // variables needed to use getTextBounds()
        uint16_t W, H;
        tft.getTextBounds(checkbox_text[page][i], x, y, &X1, &Y1, &W, &H); // Calculate text size
        tft.fillRect(checkbox_Xpos[page][i],checkbox_Ypos[page][i],size+10+W,size,page_backColors[page]); // Fill the check-box with page background color
    }
    void Gui900::changeCheckBoxChecked(int page,int i,bool val){ // Checks or unchecks a check-box
        if (checkbox_checked[page][i] != val) { // Run the code only if the value has changed
            HCT
            checkbox_checked[page][i] = val; // Assign the value
            if (CurrentPage == page){ // Use embedded draw only if it is visible
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
    void Gui900::changeCheckBoxEnabled(int page,int i,bool val){ // Enables or disables a check-box
        if (checkbox_enabled[page][i] != val) { // Just like changeLabelEnabled
            HCT
            checkbox_enabled[page][i] = val;
            if (CurrentPage == page){
                drawCheckBox(page,i);
            }
        }
    }
    void Gui900::changeCheckBoxVisible(int page,int i,bool val){ // Hides or shows a check-box
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

    int Gui900::addslider(uint16_t page,uint16_t X,uint16_t Y, uint16_t W=AUTO,uint16_t H=10,
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
            thumbWidth=H*0.7; // 70% of height. For 10px height, it is 7px.
        }
        if(W==AUTO){ // Expand AUTO macro for width
            W=tft.width()-(X*2); // Container width minus two times X position
        }
        if(max==AUTO){ // Expand AUTO macro for maximum
            max=W-thumbWidth; // Width minus thumb width
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

        return number; // Return new slider's index
    }
    void Gui900::drawSlider(uint16_t page,uint16_t i){
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
            tft.fillRect(s_X+1,s_Y+1,s_value-1,s_height-2,s_color_left); // Left area
            tft.fillRect(s_X+s_value,s_Y+1,s_thumb_width,s_height-2,s_color_thumb); // Thumb
            tft.fillRect(s_X+s_value+s_thumb_width,s_Y+1,s_width-s_value-s_thumb_width-1,s_height-2,s_color_right); // Right area
            tft.drawRect(s_X,s_Y,s_width,s_height,s_color_border); // Border
        }
    }
    void Gui900::undrawSlider(uint16_t page,uint16_t i){
        HCT
        // Get values for easier use
        uint16_t s_X            =slider_Xpos        [page][i];
        uint16_t s_width        =slider_width       [page][i];
        uint16_t s_height       =slider_height      [page][i];
        uint16_t s_Y            =slider_Ypos        [page][i];

        tft.fillRect(slider_Xpos[page][i],slider_Ypos[page][i],slider_width[page][i],slider_height[page][i],page_backColors[page]); // 'Fill' the border
    }
    int Gui900::getSliderValue(uint16_t page,uint16_t i,int min=0,int max=100){
        return map(slider_value[page][i],                               // Input value
                   0,                                                   // Input minimum
                   slider_width[page][i] - slider_thumb_width[page][i], // Input maximum
                   min,                                                 // Output minimum
                   max);                                                // Output maximum
    }

void Gui900::navigatePage( int page , int transition){ // Navigates to another page
    HCT
    switch (transition) { // Currently only one transition is supported
        case DEFAULT: // Default transition
            if(!((CurrentPage==0)&&(page==0))){ // Do not undraw anything if it was called from start()
                if (page_backColors[CurrentPage] == page_backColors[page]) { // If background colors were the same, only undraw elements, not the whole screen
                     // Undraw all buttons
                        for (int i = 0; i < button_counts[CurrentPage]; i++) {
                            undrawButton(CurrentPage, i);
                        }
                     // Undraw all labels
                        for (int i = 0; i < label_counts[CurrentPage]; i++) {
                            undrawlabel(CurrentPage, i);
                        }
                     // Undraw all check-boxes
                        for (int i = 0; i < checkbox_counts[CurrentPage]; i++) {
                            undrawCheckBox(CurrentPage, i);
                        }
                     // Undraw all sliders
                        for (int i = 0; i < slider_counts[CurrentPage]; i++) {
                            undrawSlider(CurrentPage, i);
                        }
                }
                else
                    tft.fillScreen(page_backColors[page]); // if background colors were different, fill the whole screen
            }
             // Draw all buttons
                for (int i = 0; i < button_counts[page]; i++) {
                    drawButton(page, i);
                }
             // Draw all labels
                for (int i = 0; i < label_counts[page]; i++) {
                    drawlabel(page, i);
                }
             // Draw all check-boxes
                for (int i = 0; i < checkbox_counts[page]; i++) {
                    drawCheckBox(page, i);
                }
             // Draw all sliders
                for (int i = 0; i < slider_counts[page]; i++) {
                    drawSlider(page, i);
                }
            CurrentPage = page; // Set CurrentPage
            break;
    }
}
void Gui900::checkPage( ) { // Check for touches
    if ( Touch_getXY()) { // Check elements only if somewhere is touched
            // Code to wake screen up
            if(!screenOn){ // Just if screen was off
                scrOn(); // Turn on screen
                while(Touch_getXY()); // Wait until there is no touch
                return; // Do not handle elements, so return
            }
        
            for (int i = 0; i < button_counts[CurrentPage]; i++) { // Iterate through buttons
                if (button_enabled[CurrentPage][i]) { // Disabled buttons cannot be clicked
                    uint16_t X           = button_Xpos      [CurrentPage][i]; // X
                    uint16_t Y           = button_Ypos      [CurrentPage][i]; // Y
                    uint16_t Width       = button_width     [CurrentPage][i]; // Get width
                    uint16_t Height      = button_height    [CurrentPage][i]; // Get height
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
                    button_pressed[CurrentPage][i] = inRegion(pixel_y, Y + Height, Y, pixel_x, X, X + Width); // Check if it's in the button area
                }
            }
        // Labels are only shown, they do not handle touch.
         // Handle check-boxes
            for(int i=0;i<checkbox_counts[CurrentPage];i++){ // Iterate through check-boxes
                if(checkbox_enabled[CurrentPage][i]){
                    uint16_t X           = checkbox_Xpos    [CurrentPage][i];                   // X
                    uint16_t Y           = checkbox_Ypos    [CurrentPage][i];                   // Y
                    uint16_t size        = checkbox_size    [CurrentPage][i];                   // Get height
                    // Get width
                    int16_t x = 0, y = 0, X1, Y1;                                               // variables needed to use getTextBounds()
                    uint16_t W, H;
                    tft.getTextBounds(checkbox_text[CurrentPage][i], x, y, &X1, &Y1, &W, &H);   // Calculate text size
                    uint16_t Width       = size+10+W;              // Get width
                    // Check if it was pressed
                    bool pressed=inRegion(pixel_y, Y + size , Y, pixel_x, X, X + Width);
                    if(pressed){ // Toggle states
                        if(!checkbox_lastState[CurrentPage][i]){ // Only if it wasn't being touched before
                            checkbox_checked[CurrentPage][i]=!checkbox_checked[CurrentPage][i]; // Toggle value
                            bool checked=checkbox_checked[CurrentPage][i]; // Get values for easy use
                            uint16_t checkColor=checkbox_checkColor[CurrentPage][i];
                            uint16_t boxColor  =checkbox_boxColor  [CurrentPage][i];
                            // Draw the check mark
                            int Third=size/3;
                            tft.drawLine(X+2    ,Y+(Third*2),X+Third ,Y+size-2,(checked)?checkColor:boxColor);
                            tft.drawLine(X+Third,Y+size-2   ,X+size-2,Y+1     ,(checked)?checkColor:boxColor);
                        }
                    }
                    checkbox_lastState[CurrentPage][i]=true;
                }
            }
         // Handle sliders
            for (int i = 0; i < slider_counts[CurrentPage]; i++) { // Iterate through sliders
                if (slider_enabled[CurrentPage][i]) { // Disabled sliders cannot be changed

                    uint16_t s_X                = slider_Xpos               [CurrentPage][i]; // X
                    uint16_t s_Y                = slider_Ypos               [CurrentPage][i]; // Y
                    uint16_t s_width            = slider_width              [CurrentPage][i]; // Get width
                    uint16_t s_height           = slider_height             [CurrentPage][i]; // Get height
                    uint16_t s_touch_area_height= slider_touch_area_height  [CurrentPage][i]; // Area height
                    uint16_t s_thumb_width      = slider_thumb_width        [CurrentPage][i]; // Thumb width
                    // Calculate touch region
                    int tac=s_Y+(s_height/2);      // Touch area center
                    int tah=s_touch_area_height/2; // Half of touch area height
                    int tat=tac-tah;               // Touch area top
                    int tab=tac+tah;               // Touch area botton
                    // Was the slider pressed?
                    if(inRegion(pixel_y, tab, tat, pixel_x, s_X, s_X+s_width-s_thumb_width)){
                        slider_value[CurrentPage][i]=pixel_x-s_X; // Update slider value
                        drawSlider(CurrentPage,i); // Draw slider with new value
                    }
                }
            }
    }
  else { // It wasn't pressed
        // Set all pressed states to false
        for (int k = 0; k < pageCount; k++) {
            for (int j = 0; j < button_counts[k]; j++) {
                button_pressed[k][j] = false;
            }
        }
        // Set all pressed states to false
        for (int k = 0; k < pageCount; k++) {
            for (int j = 0; j < checkbox_counts[k]; j++) {
                checkbox_lastState[k][j] = false;
            }
        }
        // Did the last touch occur in at least SCREENTIMEOUT seconds ago?
        if((timeOut>0)&&((millis()-lastTouch)>(timeOut*1000))){
            scrOff(); // Turn off screen
        }
  }
}

void Gui900::configTouch(int XP,int XM, int YP, int YM, int RX) 
{
    this->XP=XP;
    this->YP=YP;
    this->XM=XM;
    this->YM=YM;
    ts=TouchScreen(XP,YP,XM,YM,RX);
}

void Gui900::calibrateTouch(int left,int right, int top, int bottom, int min, int max) 
{
    tsLeft=left;
    tsRight=right;
    tsTop=top;
    tsBottom=bottom;
    minPressure=min;
    maxPressure=max;
}

void Gui900::timeOutEnable(int timeout) 
{
    timeOut=timeout;
}



Gui900::Gui900(int numPages,int numButtons, int numLabels, int numCheckBoxes, int numSliders):
        pageCount(numPages),
        buttonCount(numButtons),
        labelCount(numLabels),
        checkBoxCount(numCheckBoxes),
        sliderCount(numSliders){
	initarr();
}




uint16_t** Gui900::initialize2dArray(uint16_t** arr, uint16_t d1, uint16_t d2)
{
    arr=new uint16_t*[d1];
    for(uint16_t i=0;i<d1;i++){
        initArray1(arr[i],d2);
    }
	return arr;
}
bool** Gui900::initialize2dArray(bool** arr, uint16_t d1, uint16_t d2)
{
    arr=new bool*[d1];
    for(uint16_t i=0;i<d1;i++){
		initArray1(arr[i],d2);
    }
	return arr;
}
String** Gui900::initialize2dArray(String** arr, uint16_t d1, uint16_t d2)
{
    arr=new String*[d1];
    for(uint16_t i=0;i<d1;i++){
		initArray1(arr[i],d2);
    }
	return arr;
}
uint16_t* Gui900::initializeArray(uint16_t* arr, uint16_t size)
{
    arr=new uint16_t[size];
	return arr;
}
bool* Gui900::initializeArray(bool* arr, uint16_t size)
{
    arr=new bool[size];
	return arr;
}
String* Gui900::initializeArray(String* arr, uint16_t size)
{
    arr=new String[size];
	return arr;
}
void Gui900::initarr() 
{
    initArray1(button_counts,pageCount);

    initArray2(button_Xpos,pageCount,buttonCount);
    initArray2(button_Ypos,pageCount,buttonCount);
    initArray2(button_width,pageCount,buttonCount);
    initArray2(button_height,pageCount,buttonCount);
    initArray2(button_text,pageCount,buttonCount);
    initArray2(button_textColor,pageCount,buttonCount);
    initArray2(button_backColor,pageCount,buttonCount);
    initArray2(button_border,pageCount,buttonCount);
    initArray2(button_enabled,pageCount,buttonCount);
    initArray2(button_visible,pageCount,buttonCount);
    initArray2(button_radius,pageCount,buttonCount);
    initArray2(button_XTO,pageCount,buttonCount);
    initArray2(button_YTO,pageCount,buttonCount);
    initArray2(button_pressed,pageCount,buttonCount);

    
    initArray1(label_counts,pageCount);

    initArray2(label_Xpos,pageCount,labelCount);
    initArray2(label_Ypos,pageCount,labelCount);
    initArray2(label_text,pageCount,labelCount);
    initArray2(label_textColor,pageCount,labelCount);
    initArray2(label_textSizeX,pageCount,labelCount);
    initArray2(label_textSizeY,pageCount,labelCount);
    initArray2(label_enabled,pageCount,labelCount);
    initArray2(label_visible,pageCount,labelCount);

    label_font=new GFXfont**[pageCount];
    for(uint16_t i=0;i<labelCount;i++){
        label_font[i]=new GFXfont*[labelCount];
    }
    

    initArray1(checkbox_counts,pageCount);

    initArray2(checkbox_Xpos,pageCount,checkBoxCount);
    initArray2(checkbox_Ypos,pageCount,checkBoxCount);
    initArray2(checkbox_text,pageCount,checkBoxCount);
    initArray2(checkbox_size,pageCount,checkBoxCount);
    initArray2(checkbox_checkColor,pageCount,checkBoxCount);
    initArray2(checkbox_textColor,pageCount,checkBoxCount);
    initArray2(checkbox_boxColor,pageCount,checkBoxCount);
    initArray2(checkbox_checked,pageCount,checkBoxCount);
    initArray2(checkbox_enabled,pageCount,checkBoxCount);
    initArray2(checkbox_visible,pageCount,checkBoxCount);
    initArray2(checkbox_lastState,pageCount,checkBoxCount);


    initArray1(slider_counts,pageCount);

    initArray2(slider_Xpos,pageCount,sliderCount);
    initArray2(slider_Ypos,pageCount,sliderCount);
    initArray2(slider_color_border,pageCount,sliderCount);
    initArray2(slider_color_left,pageCount,sliderCount);
    initArray2(slider_color_right,pageCount,sliderCount);
    initArray2(slider_color_thumb,pageCount,sliderCount);
    initArray2(slider_height,pageCount,sliderCount);
    initArray2(slider_width,pageCount,sliderCount);
    initArray2(slider_touch_area_height,pageCount,sliderCount);
    initArray2(slider_thumb_width,pageCount,sliderCount);
    initArray2(slider_value,pageCount,sliderCount);
    initArray2(slider_enabled,pageCount,sliderCount);
    initArray2(slider_visible,pageCount,sliderCount);
    
}









