#include <Gui900.h>
Gui900 gui(6, 9,3,1,1);
#include <Fonts/FreeSerifBoldItalic24pt7b.h>    // Font: Serif, Bold, Italic
unsigned long LastPress;
byte R,G,B;
int counter_count;
void beep(int time=10){
  #ifndef __SAM3X8E__
    if(gui.checkbox_checked[5][0])
        tone(48,1000,time);
  #endif
}
void setup() {
    // Start your hardware here.
    Serial.begin(9600);
    Serial.println("Start");
    // Define controls here.
    gui.addbutton(0, 20 , 20  , 200, 60, "Color mixer");
    gui.addbutton(0, 20 , 100 , 90 , 60, "Counter"    );
    gui.addbutton(0, 20 , 180 , 200, 60, "Reaction speed test");
    gui.addbutton(0, 140, 260 , 80 , 30, "Info"       , TFT_WHITE, TFT_DARKGREY             , TFT_LIGHTGREY,true,true,CIRCLE,0,+1);
    gui.addbutton(0, 20 , 260 , 100, 30, "Settings"   , TFT_WHITE, TFT_DARKGREY             , TFT_LIGHTGREY,true,true,CIRCLE,0,-3);// Pass a text offset, because without it text will be ougui.tside button bounds
    gui.addbutton(0, 130, 100 , 90 , 60, "Paint");

    gui.addbutton(1, 20 , 20  , 60 , 60, "R-"         , TFT_WHITE, gui.tft.color565(128,0  ,0  ), TFT_LIGHTGREY);
    gui.addbutton(1, 160, 20  , 60 , 60, "R+"         , TFT_WHITE, gui.tft.color565(255,0  ,0  ), TFT_LIGHTGREY);
    gui.addbutton(1, 20 , 100 , 60 , 60, "G-"         , TFT_WHITE, gui.tft.color565(0  ,128,0  ), TFT_LIGHTGREY);
    gui.addbutton(1, 160, 100 , 60 , 60, "G+"         , TFT_BLACK, gui.tft.color565(0  ,255,0  ), TFT_LIGHTGREY);
    gui.addbutton(1, 20 , 180 , 60 , 60, "B-"         , TFT_WHITE, gui.tft.color565(0  ,0  ,128), TFT_LIGHTGREY);
    gui.addbutton(1, 160, 180 , 60 , 60, "B+"         , TFT_WHITE, gui.tft.color565(0  ,0  ,255), TFT_LIGHTGREY);
    gui.addbutton(1, 20 , 260 , 90 , 60, ""           , TFT_WHITE, TFT_BLACK);
    gui.addbutton(1, 130, 260 , 90 , 60, "Back"       , TFT_WHITE, TFT_BLACK);
    gui.addlabel (1, CENTER, 20 , "0", TFT_WHITE,&FreeSerifBoldItalic24pt7b );
    gui.addlabel (1, CENTER, 100, "0", TFT_WHITE,&FreeSerifBoldItalic24pt7b );
    gui.addlabel (1, CENTER, 180, "0", TFT_WHITE,&FreeSerifBoldItalic24pt7b );

    gui.addbutton(2,  20, 20  , 60 ,20 , "Back"       , TFT_WHITE, TFT_BLACK);
    gui.addbutton(2, 120, 200 , 110,110, "+"          );
    gui.addbutton(2,  10, 200 , 100,50 , "Undo"       , TFT_WHITE, TFT_DARKGREY, TFT_LIGHTGREY,false);
    gui.addbutton(2,  10, 260 , 100,50 , "Reset"      );
    gui.addlabel (2, CENTER,100, "0", TFT_WHITE,&FreeSerifBoldItalic24pt7b );

    gui.addbutton(3,  20, 20  , 60 ,20, "Back"       , TFT_WHITE, TFT_BLACK);
    gui.addlabel (3, 0     , 50, F("Arduino mega 2560\n"
              "Created and owned by SMHA\n"
              "MCUFRIEND ILI9325 TFT LCD shield\n"
              "Using SMHA's user interface library\n"
              "Last version 7/17/2020"), TFT_WHITE,&FreeSans9pt7b     );
    
    // Add 9 buttons
    gui.addlabel(4,0,0,"",TFT_WHITE,&FreeBigFont);
    gui.addbutton(4,0,50            ,50,50,"",TFT_BLACK,TFT_LIGHTGREY);
    gui.addbutton(4,CENTER,50       ,50,50,"",TFT_BLACK,TFT_LIGHTGREY);
    gui.addbutton(4,RIGHT,50        ,50,50,"",TFT_BLACK,TFT_LIGHTGREY);
    gui.addbutton(4,0,CENTER        ,50,50,"",TFT_BLACK,TFT_LIGHTGREY);
    gui.addbutton(4,CENTER,CENTER   ,50,50,"",TFT_BLACK,TFT_LIGHTGREY);
    gui.addbutton(4,RIGHT,CENTER    ,50,50,"",TFT_BLACK,TFT_LIGHTGREY);
    gui.addbutton(4,0,RIGHT         ,50,50,"",TFT_BLACK,TFT_LIGHTGREY);
    gui.addbutton(4,CENTER,RIGHT    ,50,50,"",TFT_BLACK,TFT_LIGHTGREY);
    gui.addbutton(4,RIGHT,RIGHT     ,50,50,"",TFT_BLACK,TFT_LIGHTGREY);

    gui.addbutton(5,10,10,100,50,"Back",TFT_WHITE,TFT_BLACK);
    gui.addcheckBox(5,20,100,"beeps",SIZE12PT28PX,true);
    gui.addslider(5,10,70,220,10,30,255,0,255,TFT_DARKGREY); // In general, this command is very simple, but when you want to specify a default value, it gegui.ts a bit complicated.
    // Initialize random
    randomSeed(analogRead(A10));
    
    gui.configTouch(6,A2,A1,7); // XP, XM, YP, YM
    gui.calibrateTouch(950,100,100,944, 10,1000); // Left, Right, Top, Bottom, min, max
    uint16_t backColors[]={0,0,0,0,0,0};
    gui.timeOutEnable(30);
    gui.start(0,backColors,46);
    gui.changeBrightness(255);
}
float mapF(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void loop() {
    gui.checkPage();
    gui.changeBrightness(gui.getSliderValue(5,0,0,255));
    // Put code for evengui.ts here
    if(gui.CurrentPage==4){
      // Pick a random button
      int num=random(9);
      unsigned long t=millis();
      // Set button color is white
      gui.changeButtonProperty(4,num,BACK_COLOR,TFT_WHITE,0);
      // Wait until button is pressed
      bool pressed=false;
      do{
        gui.checkPage();
        pressed=gui.button_pressed[4][num];
        if(gui.pixel_z<100){ //If you apply high pressure, it returns to home page. It's just an alternative to back button
            gui.navigatePage(0);
            break;
        }
      } while(!pressed);
      // Change button color back to gray
      gui.changeButtonProperty(4,num,BACK_COLOR,TFT_LIGHTGREY,0);
      gui.changeLabelText(4,0,(String(millis()-t)+"ms"));
    }
    if(gui.button_pressed[0][0]){
        beep(20);
        gui.navigatePage(1);
    }
    if(gui.button_pressed[0][1]){
        beep(20);
        gui.navigatePage(2);
    }
    if(gui.button_pressed[0][2]){
        beep(20);
        gui.navigatePage(4);
    }
    if(gui.button_pressed[0][3]){
        beep(20);
        gui.navigatePage(3);
    }
    if(gui.button_pressed[0][4]){
        beep(20);
        gui.navigatePage(5);
    }
    if(gui.button_pressed[0][5]){
        beep(20);
        runsketch(paint_setup,paint_loop);
    }


    if(gui.button_pressed[1][0]){
        beep(2);
        gui.changeLabelText(1,0,String(--R));
        gui.changeButtonProperty(1,6,  BACK_COLOR,gui.tft.color565(R,G,B),0);
        gui.changeButtonProperty(1,6,BORDER_COLOR,gui.tft.color565(R,G,B),0);
    }
    if(gui.button_pressed[1][1]){
        beep(2);
        gui.changeLabelText(1,0,String(++R));
        gui.changeButtonProperty(1,6,  BACK_COLOR,gui.tft.color565(R,G,B),0);
        gui.changeButtonProperty(1,6,BORDER_COLOR,gui.tft.color565(R,G,B),0);
    }
    if(gui.button_pressed[1][2]){
        beep(2);
        gui.changeLabelText(1,1,String(--G));
        gui.changeButtonProperty(1,6,  BACK_COLOR,gui.tft.color565(R,G,B),0);
        gui.changeButtonProperty(1,6,BORDER_COLOR,gui.tft.color565(R,G,B),0);
    }
    if(gui.button_pressed[1][3]){
        beep(2);
        gui.changeLabelText(1,1,String(++G));
        gui.changeButtonProperty(1,6,  BACK_COLOR,gui.tft.color565(R,G,B),0);
        gui.changeButtonProperty(1,6,BORDER_COLOR,gui.tft.color565(R,G,B),0);
    }
    if(gui.button_pressed[1][4]){
        beep(2);
        gui.changeLabelText(1,2,String(--B));
        gui.changeButtonProperty(1,6,  BACK_COLOR,gui.tft.color565(R,G,B),0);
        gui.changeButtonProperty(1,6,BORDER_COLOR,gui.tft.color565(R,G,B),0);
    }
    if(gui.button_pressed[1][5]){
        beep(2);
        gui.changeLabelText(1,2,String(++B));
        gui.changeButtonProperty(1,6,  BACK_COLOR,gui.tft.color565(R,G,B),0);
        gui.changeButtonProperty(1,6,BORDER_COLOR,gui.tft.color565(R,G,B),0);
    }
    if(gui.button_pressed[1][6]){
        beep(20);
        R=G=B=0;
        gui.changeLabelText(1,0,"0");
        gui.changeLabelText(1,1,"0");
        gui.changeLabelText(1,2,"0");
        gui.changeButtonProperty(1,6,  BACK_COLOR,TFT_BLACK,0);
        gui.changeButtonProperty(1,6,BORDER_COLOR,TFT_BLACK,0);
    }
    if(gui.button_pressed[1][7]){
        beep(20);
        gui.navigatePage(0);
    }
    if(gui.button_pressed[2][0]){
        beep(20);
        gui.navigatePage(0);
    }
    if(gui.button_pressed[2][1]){
        beep(10);
        counter_count++;
        gui.changeLabelText(2,0,String(counter_count));
        if(counter_count>0){
            gui.changeButtonProperty(2,2,ENABLED,true,false);
        }
        delay(1000);
    }
    if(gui.button_pressed[2][2]){
        beep(15);
        counter_count--;
        gui.changeLabelText(2,0,String(counter_count));
        if(counter_count==0){
            gui.changeButtonProperty(2,2,ENABLED,false,false);
        }
        delay(1000);
    }
    if(gui.button_pressed[2][3]){
        beep(20);
        counter_count=0;
        gui.changeLabelText(2,0,String(counter_count));
        gui.changeButtonProperty(2,2,ENABLED,false,false);
    }
    if(gui.button_pressed[3][0]){
        beep(20);
        gui.navigatePage(0);
    }
    if(gui.button_pressed[5][0]){
        beep(20);
        gui.navigatePage(0);
    }
    
}