// the regular Adafruit "TouchScreen.h" library only works on AVRs

// different mcufriend shields have Touchscreen on different pins
// and rotation.
// Run the TouchScreen_Calibr_native sketch for calibration of your shield

char *name = "Please Calibrate.";  //edit name of shield

const int TS_LEFT = 950, TS_RT = 100, TS_TOP = 100, TS_BOT = 944, XP = 6, XM = A2, YP = A1, YM = 7;
#define MINPRESSURE 10
#define MAXPRESSURE 1000
int16_t BOXSIZE;
int16_t PENRADIUS = 1;
uint16_t ID, oldcolor, currentcolor;
#include <Gui900.h>
TSPoint tp;

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void show_Serial(void)
{
    Serial.println(F("Most Touch Screens use pins 6, 7, A1, A2"));
    Serial.println(F("But they can be in ANY order"));
    Serial.println(F("e.g. right to left or bottom to top"));
    Serial.println(F("or wrong direction"));
    Serial.println(F("Edit name and calibration statemengui.ts\n"));
    Serial.println(name);
    Serial.print(F("ID=0x"));
    Serial.println(ID, HEX);
    Serial.println("Screen is " + String(gui.tft.width()) + "x" + String(gui.tft.height()));
    Serial.println("Calibration is: ");
    Serial.println("LEFT = " + String(TS_LEFT) + " RT  = " + String(TS_RT));
    Serial.println("TOP  = " + String(TS_TOP)  + " BOT = " + String(TS_BOT));
    Serial.println("Wiring is always PORTRAIT");
    Serial.println("YP=" + String(YP)  + " XM=" + String(XM));
    Serial.println("YM=" + String(YM)  + " XP=" + String(XP));
}

void show_tft(void)
{
    gui.tft.setCursor(0, 0);
    gui.tft.setTextSize(1);
    gui.tft.print(F("ID=0x"));
    gui.tft.println(ID, HEX);
    gui.tft.println("Screen is " + String(gui.tft.width()) + "x" + String(gui.tft.height()));
    gui.tft.println("");
    gui.tft.setTextSize(2);
    gui.tft.println(name);
    gui.tft.setTextSize(1);
    gui.tft.println("PORTRAIT Values:");
    gui.tft.println("LEFT = " + String(TS_LEFT) + " RT  = " + String(TS_RT));
    gui.tft.println("TOP  = " + String(TS_TOP)  + " BOT = " + String(TS_BOT));
    gui.tft.println("\nWiring is: ");
    gui.tft.println("YP=" + String(YP)  + " XM=" + String(XM));
    gui.tft.println("YM=" + String(YM)  + " XP=" + String(XP));
    gui.tft.setTextSize(2);
    gui.tft.setTextColor(RED);
    gui.tft.setCursor((gui.tft.width() - 48) / 2, (gui.tft.height() * 2) / 4);
    gui.tft.print("EXIT");
    gui.tft.setTextColor(YELLOW, BLACK);
    gui.tft.setCursor(0, (gui.tft.height() * 6) / 8);
    gui.tft.print("Touch screen for loc");
    while (1) {
        tp = gui.ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
        if (tp.x > 450 && tp.x < 570  && tp.y > 450 && tp.y < 570) break;
        gui.tft.setCursor(0, (gui.tft.height() * 3) / 4);
        gui.tft.print("tp.x=" + String(tp.x) + " tp.y=" + String(tp.y) + "   ");
    }
}


void paint_setup(void)
{
    uint16_t tmp;
    Serial.begin(9600);
    show_Serial();
    gui.tft.fillScreen(BLACK);
    gui.tft.setRotation(2);
    show_tft();

    BOXSIZE = gui.tft.width() / 10;
    gui.tft.fillScreen(BLACK);

    gui.tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
    gui.tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
    gui.tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, GREEN);
    gui.tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, CYAN);
    gui.tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, BLUE);
    gui.tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, MAGENTA);
    gui.tft.fillRect(BOXSIZE * 6, 0, BOXSIZE, BOXSIZE, BLACK);
    gui.tft.fillRect(BOXSIZE * 7, 0, BOXSIZE, BOXSIZE, WHITE);
    gui.tft.fillRect(BOXSIZE * 8, 0, BOXSIZE, BOXSIZE, TFT_DARKGREY);
    gui.tft.fillRect(BOXSIZE * 9, 0, BOXSIZE, BOXSIZE, TFT_ORANGE);

    gui.tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
    currentcolor = RED;
    delay(1000);
}

void paint_loop()
{
    uint16_t xpos, ypos;  //screen coordinates
    tp = gui.ts.getPoint();   //tp.x, tp.y are ADC values

    // if sharing pins, you'll need to fix the directions of the touchscreen pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    if(tp.z<100){
      gui.quit=true;
    }
    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
        // most mcufriend have touch (with icons) that extends below the TFT
        // screens without icons need to reserve a space for "erase"
        // scale the ADC values from gui.ts.getPoint() to screen values e.g. 0-239
        //
        // Calibration is true for PORTRAIT. tp.y is always long dimension 
        // map to your current pixel orientation
        switch (0) {
            case 0:
                xpos = map(tp.x, TS_LEFT, TS_RT, 0, gui.tft.width());
                ypos = map(tp.y, TS_TOP, TS_BOT, 0, gui.tft.height());
                break;
            case 1:
                xpos = map(tp.y, TS_TOP, TS_BOT, 0, gui.tft.width());
                ypos = map(tp.x, TS_RT, TS_LEFT, 0, gui.tft.height());
                break;
            case 2:
                xpos = map(tp.x, TS_RT, TS_LEFT, 0, gui.tft.width());
                ypos = map(tp.y, TS_BOT, TS_TOP, 0, gui.tft.height());
                break;
            case 3:
                xpos = map(tp.y, TS_BOT, TS_TOP, 0, gui.tft.width());
                ypos = map(tp.y, TS_LEFT, TS_RT, 0, gui.tft.height());
                break;
        }
        ypos=320-ypos;
        xpos=240-xpos;
        // are we in top color box area ?
        if (ypos < BOXSIZE) {               //draw white border on selected color box
            oldcolor = currentcolor;

            if (xpos < BOXSIZE) {
                currentcolor = RED;
                gui.tft.drawRect(0          , 0, BOXSIZE, BOXSIZE, WHITE);
            } else if (xpos < BOXSIZE * 2) {
                currentcolor = YELLOW;
                gui.tft.drawRect(BOXSIZE    , 0, BOXSIZE, BOXSIZE, WHITE);
            } else if (xpos < BOXSIZE * 3) {
                currentcolor = GREEN;
                gui.tft.drawRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, WHITE);
            } else if (xpos < BOXSIZE * 4) {
                currentcolor = CYAN;
                gui.tft.drawRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, WHITE);
            } else if (xpos < BOXSIZE * 5) {
                currentcolor = BLUE;
                gui.tft.drawRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, WHITE);
            } else if (xpos < BOXSIZE * 6) {
                currentcolor = MAGENTA;
                gui.tft.drawRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, WHITE);
            } else if (xpos < BOXSIZE * 7) {
                currentcolor = BLACK;
                gui.tft.drawRect(BOXSIZE * 6, 0, BOXSIZE, BOXSIZE, WHITE);
            } else if (xpos < BOXSIZE * 8) {
                currentcolor = WHITE;
                gui.tft.drawRect(BOXSIZE * 7, 0, BOXSIZE, BOXSIZE, WHITE);
            } else if (xpos < BOXSIZE * 9) {
                currentcolor = TFT_DARKGREY;
                gui.tft.drawRect(BOXSIZE * 8, 0, BOXSIZE, BOXSIZE, WHITE);
            } else if (xpos < BOXSIZE * 10) {
                currentcolor = TFT_ORANGE;
                gui.tft.drawRect(BOXSIZE * 9, 0, BOXSIZE, BOXSIZE, WHITE);
            }

            if (oldcolor != currentcolor) { //rub out the previous white border
                if (oldcolor == RED) gui.tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
                if (oldcolor == YELLOW) gui.tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
                if (oldcolor == GREEN) gui.tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, GREEN);
                if (oldcolor == CYAN) gui.tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, CYAN);
                if (oldcolor == BLUE) gui.tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, BLUE);
                if (oldcolor == MAGENTA) gui.tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, MAGENTA);
                if (oldcolor == BLACK) gui.tft.fillRect(BOXSIZE * 6, 0, BOXSIZE, BOXSIZE, BLACK);
                if (oldcolor == WHITE) gui.tft.fillRect(BOXSIZE * 7, 0, BOXSIZE, BOXSIZE, WHITE);
                if (oldcolor == TFT_DARKGREY) gui.tft.fillRect(BOXSIZE * 8, 0, BOXSIZE, BOXSIZE, TFT_DARKGREY);
                if (oldcolor == TFT_ORANGE) gui.tft.fillRect(BOXSIZE * 9, 0, BOXSIZE, BOXSIZE, TFT_ORANGE);
            }
        }
        // are we in drawing area ?
        if (((ypos - PENRADIUS) > BOXSIZE) && ((ypos + PENRADIUS) < gui.tft.height())) {
            gui.tft.fillCircle(xpos, ypos, PENRADIUS, currentcolor);
        }
        // are we in erase area ?
        // Plain Touch panels use bottom 10 pixels e.g. > h - 10
        // Touch panels with icon area e.g. > h - 0
        if (ypos < 0) {
            // press the bottom of the screen to erase
            gui.tft.fillRect(0, BOXSIZE, gui.tft.width(), gui.tft.height() - BOXSIZE, BLACK);
        }
    }
}