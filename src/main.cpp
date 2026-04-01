#include <Arduino.h>
// #include <TFT_eSPI.h>
#include <globals.h>
#include <display_funcs.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESP32Encoder.h>
#include <encoder_funcs.h>
#include <funcs.h>
#include "LGFX_ILI9488.hpp"
#include <time.h>
#include <ESP32Ping.h>

// TFT_eSPI tft = TFT_eSPI();
LGFX lcd;
String current_tab = "STATS";
std::vector<String> menu_tabs = {"STATS", "SHELL", "SYSTEM"};
int selected = 1;
int counter = 0;
ESP32Encoder encoder;
long last_step = 0;

char state_enc;

bool in_menu = true;

hw_timer_t * timer = NULL;

int current_time[6];
const char* ntp_server = "216.239.35.0";
int counter_update = 0;

// canvas is for menu, all other canvases are always changing their purposes
LGFX_Sprite canvas(&lcd);

LGFX_Sprite canvas1(&lcd);
LGFX_Sprite canvas2(&lcd);

const String SSID = "";
const String PASSWD = "";

// for AP that esp32 will host
const String AP_SSID = "pipboy_ap";
const String AP_PASSWD = "pipboy12";

// open weather info
const char* api_key_temp = "";
const char* city = "Zagreb,HR";

void IRAM_ATTR encoder_checker() {
    read_enc();
}

void setup(){
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    /*
    tft.init();
    tft.setRotation(1);
    tft.invertDisplay(true); 
    tft.setTextColor(0x1FE7, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    */

    lcd.init();
    lcd.setRotation(1);
    lcd.setBrightness(200);

    draw_one_frame();

    if (!LittleFS.begin()) {
        Serial.println("LittleFS Mount Failed!");
        return;
    }

    pinMode(ENC_SW, INPUT_PULLUP);

    ESP32Encoder::useInternalWeakPullResistors = UP;
    encoder.attachFullQuad(ENC_CLK, ENC_DT);

    encoder.setCount(0);

    // Connecting to AP for internet connection
    bool res = connect_to_AP(SSID, PASSWD);

    if (res == false){
        Serial.println("Cant connect to AP");
    }

    IPAddress ip = WiFi.localIP();

    Serial.println("ESP's IP address: " + ip.toString());

    // Getting current time
    configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", ntp_server);

    struct tm timeinfo;

    while (!getLocalTime(&timeinfo)) {
        Serial.print(".");
        delay(500); 
    }
    Serial.println("\ninternal clock set");

    timer = timerBegin(0, 80, true); 
    timerAttachInterrupt(timer, &encoder_checker, true);
    timerAlarmWrite(timer, 10000, true);
    timerAlarmEnable(timer);

    // creating sprites
    canvas.createSprite(300, 30);
}

void loop(){
    if (state_enc != 'n'){
        if (state_enc == 'u'){
            selected++;
        }
        else {
            selected--;
        }

        if (selected == menu_tabs.size()) {
            selected = 0;
        }
        else if (selected == -1){
            selected = menu_tabs.size() - 1;
        }

        Serial.println("Drawing one frame");
    }

    if (counter_update % 100 == 0){
        update_weather();

        Serial.println(current_temp);
    }

    if (counter_update % 5 == 0){
        get_local_time();

        Serial.println(current_time[0]);
        Serial.println(current_time[1]);
        Serial.println(current_time[2]);
        Serial.println(current_time[3]);
        Serial.println(current_time[4]);
        Serial.println(current_time[5]);
    }

    draw_one_frame();

    counter_update++;

    if (counter_update > 1000){
        counter_update = 0;
    }

    delay(50);
}