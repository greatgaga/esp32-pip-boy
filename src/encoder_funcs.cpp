#include <ESP32Encoder.h>
#include <Arduino.h>
#include <globals.h>
#include <encoder_funcs.h>

void read_enc() {
    long current_steps = (long)encoder.getCount();   
    static int64_t last_time = 0;
    int64_t now = esp_timer_get_time();
    
    if (now - last_time < 30000) {
        return;
    }
    last_time = now;

    state_enc = 'n'; 

    if (current_steps < last_step) {
        state_enc = 'd';
    }
    else if (current_steps > last_step) {
        state_enc = 'u';
    }

    last_step = current_steps;
}