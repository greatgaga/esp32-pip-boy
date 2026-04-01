#include <funcs.h>
#include <WiFi.h>
#include <time.h>
#include <globals.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// helper

void test_ping() {
    Serial.print("Pinging Google DNS (8.8.8.8): ");
    
    if (Ping.ping("8.8.8.8")) {
        Serial.println("Success!");
    } else {
        Serial.println("Failed. Packet lost.");
    }
}


bool connect_to_AP(String ssid, String passwd){
    WiFi.begin(ssid.c_str(), passwd.c_str());

    WiFi.setSleep(false);

    int timeout_counter = 0;

    while (WiFi.status() != WL_CONNECTED && timeout_counter < 20){
        timeout_counter++;

        Serial.print('.');

        delay(500);
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED){
        test_ping();

        return true;
    }
    else {
        return false;
    }
}

bool host_AP(String AP_SSID, String AP_PASSWD){
    if (WiFi.softAP(AP_SSID.c_str(), AP_PASSWD.c_str())){
        return true;
    }
    else {
        return false;
    }
}

void get_local_time(){
    struct tm timeinfo;

    static int info[6];

    if (!getLocalTime(&timeinfo)){
        Serial.println("failed to obtain time");

        return;
    }
    
    current_time[0] = timeinfo.tm_hour;
    current_time[1] = timeinfo.tm_min;
    current_time[2] = timeinfo.tm_sec;

    current_time[3] = timeinfo.tm_mday;
    current_time[4] = timeinfo.tm_mon + 1;
    current_time[5] = timeinfo.tm_year + 1900;
}

void update_weather(){
    if (WiFi.status() == WL_CONNECTED){
        HTTPClient client;

        String url = "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "&appid=" + String(api_key_temp) + "&units=metric";

        client.begin(url);

        int http_code = client.GET();

        if (http_code > 0) {
            String payload = client.getString();

            JsonDocument doc;

            deserializeJson(doc, payload);

            current_temp = doc["main"]["temp"];
        }

        client.end();
    }
}