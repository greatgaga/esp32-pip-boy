// #include <TFT_eSPI.h>
#include <Arduino.h>
#include <globals.h>
#include "LGFX_ILI9488.hpp"
// #include <display_funcs.h>

#define PIPBOY_GREEN 0x1FE7

/*
void draw_one_frame(){
    tft.fillRect(0, 15, 480, 30, TFT_BLACK);

    tft.setCursor(20, 20);

    tft.setTextSize(2);

    tft.setTextColor(PIPBOY_GREEN, TFT_BLACK);

    Serial.println(selected);

    draw_menu();
}

void draw_menu(){
    tft.setCursor(20, 20);

    for (int i = 0; i < menu_tabs.size(); i++){
        if (i == selected){
            //Serial.println("Here");
            underline(tft.getCursorX(), tft.getCursorY(), menu_tabs[i], 2);
        }

        if (i < menu_tabs.size() - 1){
            tft.print(menu_tabs[i] + " | ");
        }
        else{
            tft.print(menu_tabs[i]);
        }
        //Serial.println("not here");
    }
}
*/

// helper

void underline(int x, int y, String word, int font_size){
    canvas.setCursor(x, y);

    int width = canvas.textWidth(word);

    canvas.drawFastHLine(x, y + (8 * font_size), width, PIPBOY_GREEN);

    canvas.setCursor(x, y);
}

// funcs

void draw_stats(){
    canvas1.setColorDepth(8);

    canvas1.createSprite(250, 200);

    canvas1.fillScreen(TFT_BLACK);

    canvas1.setTextColor(PIPBOY_GREEN);

    canvas1.setTextSize(3);

    canvas1.setCursor(10, 20);

    // adding time
    char buff[30];

    snprintf(buff, sizeof(buff), "%02d.%02d.%04d\n%02d:%02d:%02d", current_time[3], current_time[4], current_time[5], current_time[0], current_time[1], current_time[2]);

    canvas1.print(buff);

    // adding outside temp.
    canvas1.print("\nTemp: " + String(current_temp) + "\n");

    // adding current battery %
    canvas1.print("Battery %: \n");

    // adding temp. of CPU
    canvas1.print("CPU temp: " + String(current_cpu_temp));

    canvas1.pushSprite(10, 60);
}

void draw_menu(){
    canvas.setColorDepth(8);

    canvas.fillScreen(TFT_BLACK);

    canvas.setTextColor(PIPBOY_GREEN);

    canvas.setTextSize(2);

    canvas.setCursor(10, 10);

    for (int i = 0; i < menu_tabs.size(); i++){
        if (i == selected){
            underline(canvas.getCursorX(), canvas.getCursorY(), menu_tabs[i], 2);
        }
        canvas.print(menu_tabs[i]);

        if (i != menu_tabs.size() - 1){
            canvas.print(" | ");
        }
    }

    canvas.pushSprite(0, 0);

    // drawing chosen option
    if (current_tab == "STATS"){
        draw_stats();
    }
}

void draw_one_frame(){
    if (in_menu == true){
        Serial.println("Drawing menu");

        draw_menu();
    }
}