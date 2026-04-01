#ifndef FUNCS_H
#define FUNCS_H

#include <Arduino.h>
#include <WiFi.h>
#include <globals.h>

bool connect_to_AP(String ssid, String passwd);
void get_local_time();
void test_ping();
void update_weather();

#endif